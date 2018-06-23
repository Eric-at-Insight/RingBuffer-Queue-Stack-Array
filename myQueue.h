/*
 * myQueue.h
 *
 *  Created on: Feb 21, 2018
 *      Author: Eric Belanger
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t DepthMax;
    uint32_t MsgSize;
    void * pHead;
    void * pTail;
    void * pFirst;
    void * pLast;
}Queue;

void QueueCreate(Queue * pQueue, uint32_t QueueMaxDepth, uint32_t MsgSize, uint8_t * pMsgArray);
bool QueueReset(Queue * pQueue);
bool QueuePush(Queue * pQueue, const void * pMsg);
bool QueuePushUnique(Queue * pQueue, const void * pMsg, const void * pMsgMember, uint32_t MsgMemeberSize);
bool QueueFIFO(Queue * pQueue, void * pMsg);
bool QueueLIFO(Queue * pQueue, void * pMsg);
bool QueueIsEmpty(const Queue * pQueue);
bool QueueInsert(Queue * pQueue, void * pMsgPlace, const void * pMsg);
bool QueueDelete(Queue * pQueue, void * pMsgPlace);
bool QueuePeekFirst(const Queue * pQueue, void ** pMsgFirst);
bool QueuePeekLast(const Queue * pQueue, void ** pMsgLast);
uint32_t QueueGetDepth (const Queue * pQueue);
uint32_t QueueSearch(const Queue * pQueue, const void * pMsgElement, const void * pMsgMember, uint32_t MsgMemeberSize, void ** pMsgResult, uint32_t * MsgPlace);
bool QueueSearchByFunc(const Queue * pQueue, const void * pMsgElement, const void * pMsgMember, bool (*pFunc)(void *), void ** pMsgResult);
bool pfSearchRule(void *);

#endif
