/*
 * Testing.c
 *
 *  Created on: Feb 21, 2018
 *      Author: Eric Belanger
 */

#include "myQueue.h"

typedef struct
{
	uint32_t member1;
	uint8_t member2;
	uint16_t member3;
}a_struct;

Queue testQueue;
a_struct testMsgArray[10];
a_struct sandbox;

bool pfSearchRule(void *);

int main(void)
{
	uint32_t occurances = 0;
	uint32_t place = 0;
	a_struct * pMsgFound = NULL;
	bool pass = false;
	QueueCreate(&testQueue, 10, sizeof(a_struct), (uint8_t*)testMsgArray);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePushUnique(&testQueue, &sandbox, &sandbox.member1, sizeof(a_struct));
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePushUnique(&testQueue, &sandbox, &sandbox.member1, sizeof(a_struct));
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueueInsert(&testQueue, &testMsgArray[5], &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueueInsert(&testQueue, &testMsgArray[5], &sandbox);
	pass = QueueDelete(&testQueue, &testMsgArray[2]);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueueFIFO(&testQueue, &sandbox);
	pass = QueueFIFO(&testQueue, &sandbox);
	pass = QueueFIFO(&testQueue, &sandbox);
	pass = QueueFIFO(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueuePush(&testQueue, &sandbox);
	pass = QueueFIFO(&testQueue, &sandbox);
	pass = QueueLIFO(&testQueue, &sandbox);
	occurances = QueueSearch(&testQueue, &sandbox, &sandbox.member1, sizeof(sandbox.member1), (void *)&pMsgFound, &place);
	occurances = QueueSearch(&testQueue, &sandbox, &sandbox.member1, sizeof(sandbox.member1), (void *)&pMsgFound, &place);
	pass = QueueSearchByFunc(&testQueue, &sandbox, &sandbox.member1, pfSearchRule, (void *)&pMsgFound);
	QueueReset(&testQueue);
	pass = QueueIsEmpty(&testQueue);
	return 0;
}

bool pfSearchRule(void * pWalkingMsg)
{
	if (*(uint32_t *)pWalkingMsg > 8000)
	{
		return true;
	}
	return false;
}
