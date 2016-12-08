//
// Created by sethw on 11/29/2016.
//

#include <stdio.h>
#include <stdlib.h>

#include "scheduler.h"
#include "alap.h"
#include "lists.h"

#define NUM_RESOURCES 3 // 0 is add/sub, 1 is logic, 2 is mult

operationArrayNode* getCandidates(operationArrayNode** unscheduled, int resourceType, int currentCycle);
int isCandidateOp(operationArrayNode** unscheduled, operationArrayNode* check, int resourceType, int currentCycle);
int computeSlack(operationArrayNode* candidate,  int curCycle);
void scheduleNode(operationArrayNode** unscheduled, slackNode** slackNodes, slackNode* opNode,int curCycle,
                  int curResource, int* totalResources, int* inUseResources);
void removeFromUnscheduled(operationArrayNode** unscheduled, operationArrayNode* remove);
void freeSlackNodes(slackNode* head);

int startListR(mainContainer* mContainer) {
    operationArrayNode* unscheduled = NULL, *tempArrayNode = NULL, *candidates = NULL, *canTemp;
    operationNode* tempOpNode = mContainer->operations;
    operationArrayNode* slots[mContainer->maxLatency];
    slackNode* slackHead = NULL, *slackTemp, *lowestALAP = NULL;
    int curResourceType, time = 1;
    int resources[NUM_RESOURCES] = {1, 1, 1};
    int inUseResources[NUM_RESOURCES] = {0, 0, 0};

    //set ALAP time
    if (!setALAP(mContainer)){
        printf("\nCOULD NOT SCHEDULE ALL NODES WITHIN LATENCY CONSTRAINT\n");
        return -1;
    }

    //add all nodes to unscheduled list
    while (tempOpNode != NULL){
        tempArrayNode = (operationArrayNode*)malloc(sizeof(operationArrayNode));
        tempArrayNode->element = tempOpNode;
        tempArrayNode->next = NULL;
        addToOpArrayList(&unscheduled, tempArrayNode);
        tempOpNode = tempOpNode->next;
    }

    while (unscheduled != NULL){
        //for each resource
        for (curResourceType = 0; curResourceType < NUM_RESOURCES; curResourceType++){

            if (candidates != NULL) freeOpArrayNodeList(candidates);
            if (slackHead != NULL) {
                freeSlackNodes(slackHead);
                slackHead = NULL;
            }
            //get all the candidate operations
            candidates = getCandidates(&unscheduled, curResourceType, time);
            //if there are no candidates, move to the next resource type
            if (candidates == NULL) continue;
            canTemp = candidates;
            //for each candidate
            //calculate the slack
            while (canTemp != NULL){
                //make a slack node yo
                slackTemp = (slackNode*)malloc(sizeof(slackNode));
                slackTemp->element = canTemp;
                slackTemp->slack = computeSlack(canTemp, time);
                slackTemp->next = NULL;
                addToSlackList(&slackHead, slackTemp);

                canTemp = canTemp->next;
            }

            slackTemp = slackHead;

            //schedule all nodes that need to be scheduled
            while (slackTemp != NULL){
                //if slack is 0 then we need to schedule
                if (slackTemp->slack == 0){
                    scheduleNode(&unscheduled, &slackHead, slackTemp, time,
                                 curResourceType, resources, inUseResources);
                    slackTemp->element->element->listRCycle = time;
                    addOpToSchedule(&(mContainer->scheduledNodes[time-1]), slackTemp->element->element);
                }
                slackTemp = slackTemp->next;
            }
            if(slackHead == NULL) continue;
            //while we still have resources available
            while (inUseResources[curResourceType] < resources[curResourceType]) {
                slackTemp = slackHead;
                //for each candidate operation
                while (slackTemp != NULL) {
                    //find the operation with the lowest alapTime
                    if (lowestALAP == NULL || slackTemp->element->element->alapCycle < lowestALAP->element->element->alapCycle){
                        lowestALAP = slackTemp;
                    }
                    slackTemp = slackTemp->next;
                }
                //schedule operation with lowest alapTime
                scheduleNode(&unscheduled, &slackHead, lowestALAP, time,
                             curResourceType, resources, inUseResources);
                //moved this outside of function because it wasn't working but now i'm too lazy
                //to put it back into it. so its staying here.
                lowestALAP->element->element->listRCycle = time;
                addOpToSchedule(&(mContainer->scheduledNodes[time-1]), lowestALAP->element->element);
            }
            lowestALAP = NULL;
        }

        //reset the in use resources and increment time slot
        inUseResources[0] = 0;
        inUseResources[1] = 0;
        inUseResources[2] = getInUseMultipliers(mContainer->operations, time);
        time++;
    }
    return 0;
}

operationArrayNode* getCandidates(operationArrayNode** unscheduled,  int resourceType, int currentCycle){
    operationArrayNode* head = NULL, *tempArrayNode = *unscheduled, *tempAdd;

    //for each unscheduled node
    while (tempArrayNode != NULL){
        //check if it is a candidate
        if (isCandidateOp(unscheduled, tempArrayNode, resourceType, currentCycle)){
            //if it is make a new node and add it to the candidate list
            tempAdd = (operationArrayNode*)malloc(sizeof(operationArrayNode));
            tempAdd->element = tempArrayNode->element;
            tempAdd->next = NULL;
            addToOpArrayList(&head, tempAdd);
        }

        tempArrayNode = tempArrayNode->next;
    }

    return  head;
}

int isCandidateOp(operationArrayNode** unscheduled, operationArrayNode* check, int resourceType, int currentCycle){
    operationArrayNode* tempArrayNode = *unscheduled, *tempDep = check->element->dependencies;

    //if the node isn't of the right type
    if (check->element->opType != resourceType) return 0;

    while (tempDep != NULL){

        //if we find a dependency that hasn't been scheduled
        if (tempDep->element->listRCycle == -1) return 0;
        //if the dependency was scheduled in this cycle, we have to wait until next cycle for it to finish
        if (tempDep->element->listRCycle == currentCycle) return 0;
        //if the dependency is a multiplier and was scheduled in the previous cycle, we have to wait another cycle
        if (tempDep->element->opType == 2 && tempDep->element->listRCycle == currentCycle-1) return 0;

        tempDep = tempDep->next;
    }
    return 1;
}

int computeSlack(operationArrayNode* candidate,  int curCycle){
    return candidate->element->alapCycle - curCycle;
}

void scheduleNode(operationArrayNode** unscheduled, slackNode** slackNodes, slackNode* scheduleNode,int curCycle,
                  int curResource, int* totalResources, int* inUseResources){

    if (inUseResources[curResource] < totalResources[curResource]){
        inUseResources[curResource]++;
    } else if (inUseResources[curResource] == totalResources[curResource]){
        inUseResources[curResource]++;
        totalResources[curResource]++;
    }
    removeFromUnscheduled(unscheduled, scheduleNode->element);
    removeFromSlackNodes(slackNodes, scheduleNode);
}

void removeFromUnscheduled(operationArrayNode** unscheduled, operationArrayNode* remove){
    operationArrayNode *curElement, *tempElement, *prevElement, *head;
    curElement = *unscheduled;

    while (curElement != NULL){
        //if the current element is the one we want to remove
        if (curElement->element == remove->element){
            //if the unscheduled head is the one we want to remove,
            if ((*unscheduled)->element == remove->element){
                //make the head point to the next
                *unscheduled = (*unscheduled)->next;

            }else{
                prevElement->next = curElement->next;
            }
            return;
        }
        prevElement = curElement;
        curElement = curElement->next;
    }
}

void removeFromSlackNodes(slackNode** slackNodes, slackNode* remove){
    slackNode *curElement, *tempElement, *prevElement, *head;
    curElement = *slackNodes;

    while (curElement != NULL){
        //if the current element is the one we want to remove
        if (curElement->element->element == remove->element->element){
            //if the slack head is the one we want to remove,
            if ((*slackNodes)->element->element == remove->element->element){
                //make the head point to the next
                *slackNodes = (*slackNodes)->next;

            }else{
                prevElement->next = curElement->next;
            }
            return;
        }
        prevElement = curElement;
        curElement = curElement->next;
    }
}

void freeSlackNodes(slackNode* head){
    slackNode *temp;
    while (head!=NULL){
        temp = head->next;
        free(head);
        head = temp;
    }
}

int getInUseMultipliers(operationNode* operations, int curCycle){
    operationNode* temp = operations;
    int count = 0;

    //go through each node and see if
    while(temp!=NULL){
        if (temp->opType == 2 && temp->listRCycle == curCycle){
            count++;
        }

        temp=temp->next;
    }
    return count;
}
