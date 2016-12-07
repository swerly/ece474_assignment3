//
// Created by sethw on 12/4/2016.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

int hasNoDependents(operationArrayNode* check, operationArrayNode** unsched, int curCycle);
void removeFromUnscheduledALAP(operationArrayNode** scheduled, operationArrayNode** unscheduled, operationArrayNode* remove);

int setALAP(mainContainer* container){
    operationArrayNode *scheduled = NULL, *unscheduled = NULL;
    operationArrayNode* tempArrayOp = NULL;
    operationNode* tempOperation = container->operations;
    int curSlot= container->maxLatency;

    //add all nodes to unscheduled list
    while (tempOperation != NULL){
        tempArrayOp = (operationArrayNode*)malloc(sizeof(operationArrayNode));
        tempArrayOp->element = tempOperation;
        tempArrayOp->next = NULL;
        addToOpArrayList(&unscheduled, tempArrayOp);
        tempOperation = tempOperation->next;
    }

    //while there are slots to be filled (this will help us find if we cant schedule everything within the time)
    while (curSlot > 0){
        tempArrayOp = unscheduled;
        while (tempArrayOp != NULL){
            if (hasNoDependents(tempArrayOp, &unscheduled, curSlot)){
                tempArrayOp->element->alapCycle = curSlot;
                removeFromUnscheduledALAP(&scheduled, &unscheduled, tempArrayOp);
            }

            tempArrayOp = tempArrayOp->next;
        }

        curSlot--;
    }
    curSlot = unscheduled == NULL;
    //freeLists(scheduled, unscheduled);
    return curSlot;
}

int hasNoDependents(operationArrayNode* check, operationArrayNode** unsched, int curCycle){
    operationArrayNode* tempNode, *tempDep = check->element->dependents;

    if (check->element->dependents == NULL) {
        return 1;
    }

    //for each dependency
    while (tempDep != NULL){
        tempNode = *unsched;
        while (tempNode != NULL){
            //handle multipliers taking 2 cycles
            if (check->element->opType == 2 && curCycle == (tempDep->element->alapCycle - 1)) return 0;
            //if the dependency has been scheduled in the same cycle as this one
            if (tempDep->element->alapCycle == curCycle) return 0;
            //if we find a dependency that hasn't been scheduled
            if (tempDep->element == tempNode->element) return 0;
            tempNode = tempNode->next;
        }
        tempDep = tempDep->next;
    }

    return 1;
}

void removeFromUnscheduledALAP(operationArrayNode** scheduled, operationArrayNode** unscheduled, operationArrayNode* remove){
    operationArrayNode *curElement, *tempElement, *prevElement, *schedElement;

    curElement = *unscheduled;

    while (curElement != NULL){
        //if the current element is the one we want to remove
        if (curElement == remove){
            //if the unscheduled head is the one we want to remove,
            if (*unscheduled == remove){
                //make the head point to the next
                *unscheduled = remove->next;

            }else{
                //else make the previous element point to the next element of the remove node
                tempElement = remove->next;
                prevElement->next = tempElement;
            }
            schedElement = (operationArrayNode*)malloc(sizeof(operationArrayNode));
            schedElement->next = NULL;
            schedElement->element = remove->element;
            addToOpArrayList(scheduled, schedElement);
            return;
        }
        prevElement = curElement;
        curElement = curElement->next;
    }


}

void freeLists(operationArrayNode* s, operationArrayNode* u){
    operationArrayNode* t = s;

    while (t != NULL){
        t = s->next;
        free(s);
    }
    t = u;
    while (t != NULL){
        t = u->next;
        free(u);
    }
}