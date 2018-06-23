/*
 * myQueue.c
 *
 *  Created on: Feb 21, 2018
 *      Author: Eric Belanger
 */

#include "myQueue.h"

void QueueCreate(Queue *pQueue, uint32_t QueueMaxDepth, uint32_t MsgSize, uint8_t *pMsgArray)
{
        pQueue->pHead = pMsgArray;
        pQueue->pFirst = pMsgArray;
        pQueue->pLast = pMsgArray;
        pQueue->pTail = pQueue->pHead + (QueueMaxDepth - 1) * MsgSize;
        pQueue->MsgSize = MsgSize;
        pQueue->DepthMax = QueueMaxDepth;
}

bool QueueReset(Queue * pQueue)
{
    if (pQueue->pTail == pQueue->pHead + ( pQueue->DepthMax - 1) * pQueue->MsgSize)
    {
    	pQueue->pFirst = pQueue->pHead;
    	pQueue->pLast = pQueue->pHead;
    	return true;
    }
	return false;
}

bool QueueValid(const Queue * pQueue)
{
    if ( pQueue->pTail == pQueue->pHead + ( pQueue->DepthMax - 1) * pQueue->MsgSize
    		&& pQueue->pFirst >= pQueue->pHead
    		&& pQueue->pFirst <= pQueue->pTail
    		&& (( pQueue->pLast >= pQueue->pHead && pQueue->pLast <= pQueue->pTail )
    		|| pQueue->pLast == NULL ))
    {
    	return true;
    }
    return false;
}

bool QueuePush(Queue * pQueue, const void * pMsg)
{
    if (pQueue->pLast != NULL && QueueValid(pQueue))
    {
		memcpy( pQueue->pLast, pMsg, pQueue->MsgSize );
		if( pQueue->pLast >= pQueue->pTail)
		{
			 pQueue->pLast = pQueue->pHead;
		}
		else
		{
			pQueue->pLast += pQueue->MsgSize;
		}
		if (pQueue->pLast == pQueue->pFirst)
		{
			pQueue->pLast = NULL;
		}
		return true;
    }
    return false;
}

bool QueuePushUnique(Queue * pQueue, const void * pMsg, const void * pMsgMember, uint32_t MsgMemeberSize)
{
    bool bReturn = false;
    if (pQueue->pLast != NULL)
    {
    	bool found = false;
    	uint32_t depth = QueueGetDepth(pQueue);
    	if ( depth != -1)
    	{
    		uint32_t x;
			uint32_t Offset = pMsgMember - pMsg;
			void * pWalk = pQueue->pFirst;
			for ( x = 0; x < depth; x++ )
			{
				if (strncmp(pWalk + Offset, pMsgMember, MsgMemeberSize) == 0)
				{
					if (found)
					{
						QueueDelete(pQueue, pWalk);
						if ( pQueue->pLast > pQueue->pFirst || pWalk < pQueue->pFirst )
						{
							continue;
						}
					}
					else
					{
						found = true;
						bReturn = true;
					}
				}
				if(pWalk >= pQueue->pTail)
				{
					pWalk = pQueue->pHead;
				}
				else
				{
					pWalk += pQueue->MsgSize;
				}
			}
			if (!found)
			{
				memcpy( pQueue->pLast, pMsg, pQueue->MsgSize );
				if( pQueue->pLast >= pQueue->pTail)
				{
					 pQueue->pLast = pQueue->pHead;
				}
				else
				{
					pQueue->pLast += pQueue->MsgSize;
				}
				if (pQueue->pLast == pQueue->pFirst)
				{
					pQueue->pLast = NULL;
				}
				bReturn = true;
			}
    	}

    }
    return bReturn;
}

bool QueueFIFO(Queue * pQueue, void * pFirst)
{
    if(pQueue->pFirst != pQueue->pLast &&  QueueValid(pQueue))
    {
    	memcpy(pFirst, pQueue->pFirst, pQueue->MsgSize );
    	if (pQueue->pLast == NULL)
    	{
    		pQueue->pLast = pQueue->pFirst;
    	}
    	if ( pQueue->pFirst >= pQueue->pTail)
    	{
    		pQueue->pFirst = pQueue->pHead;
    	}
    	else
    	{
    		pQueue->pFirst += pQueue->MsgSize;
    	}
        return true;
    }
    return false;
}

bool QueueLIFO(Queue * pQueue, void * pLast)
{
    if(pQueue->pFirst != pQueue->pLast && QueueValid(pQueue))
    {
    	if (pQueue->pLast !=  pQueue->pHead)
    	{
   			pQueue->pLast -= pQueue->MsgSize;
    	}
    	else if (pQueue->pLast == NULL && pQueue->pFirst != pQueue->pHead)
    	{
    		pQueue->pLast = pQueue->pFirst - pQueue->MsgSize;
    	}
    	else
    	{
    		pQueue->pLast = pQueue->pTail;
    	}
        memcpy(pLast, pQueue->pLast, pQueue->MsgSize );
        return true;
    }
    return false;
}

bool QueueIsEmpty(const Queue * pQueue)
{
    if (pQueue->pFirst == pQueue->pLast && QueueValid(pQueue))
    {
        return true;
    }
    return false;
}

uint32_t QueueGetDepth(const Queue * pQueue)
{
	uint32_t depth = 0;
	if ( QueueValid(pQueue))
	{
		if ( pQueue->pLast == NULL)
		{
			depth = pQueue->DepthMax;
		}
		else if (pQueue->pFirst != pQueue->pLast)
		{
			if (pQueue->pLast > pQueue->pFirst)
			{
				depth = (pQueue->pLast - pQueue->pFirst) / pQueue->MsgSize;
			}
			else
			{
				depth = ((pQueue->pLast - pQueue->pHead) + (pQueue->pTail - pQueue->pFirst)) / pQueue->MsgSize + 1;
			}
		}
	}
	else
	{
		depth = -1;
	}
    return depth;
}

uint32_t QueueSearch(const Queue * pQueue, const void * pMsgElement, const void * pMsgMember, uint32_t MsgMemeberSize, void ** pMsgResult, uint32_t * Msg1stFound)
{
	uint32_t occurances = 0;
	if (pQueue->pFirst != pQueue->pLast)
	{
		uint32_t x;
		uint32_t depth = QueueGetDepth(pQueue);
		if ( depth != -1)
		{
			uint32_t Offset = pMsgMember - pMsgElement;
			void * pWalk = pQueue->pFirst;
			for (x = 0; x < depth; x++)
			{
				if (strncmp(pWalk + Offset, pMsgMember, MsgMemeberSize) == 0)
				{
					if (!occurances)
					{
						*Msg1stFound = x + 1;
						*pMsgResult = pWalk;
					}
					occurances++;
				}
				if(pWalk >= pQueue->pTail)
				{
					pWalk = pQueue->pHead;
				}
				else
				{
					pWalk += pQueue->MsgSize;
				}
			}
			if (occurances == 0)
			{
				*pMsgResult = NULL;
				*Msg1stFound = 0;
			}
		}
		else
		{
			occurances = -1;
		}
	}
	return occurances;
}

bool QueueDelete(Queue * pQueue, void * pMsg)
{
// WIP
//    bool bReturn = false;
//    if ( pQueue->pFirst != pQueue->pLast && QueueValid(pQueue))
//    {
//    	if (pQueue->pLast == NULL)
//    	{
//    		if ( pMsg == pQueue->pFirst )
//    		{
//    			pQueue->pLast = pQueue->pFirst;
//    			if ( pQueue->pFirst == pQueue->pTail)
//    			{
//    				pQueue->pFirst = pQueue->pHead;
//    			}
//    			else
//    			{
//    				pQueue->pFirst += pQueue->MsgSize;
//    			}
//    		}
//    		if ( pMsg > pQueue->pFirst )
//    		{
//    			pQueue->pLast = pQueue->pFirst;
//
//    		}
//    	}
//    }
    return false;
}

bool QueuePeekFirst(const Queue * pQueue, void ** pMsgFirst)
{
    if (pQueue->pFirst != pQueue->pLast && QueueValid(pQueue))
    {
    	*pMsgFirst = pQueue->pFirst;
    	return true;
    }
    return false;
}

bool QueuePeekLast(const Queue * pQueue, void ** pMsgLast)
{
    if (pQueue->pFirst != pQueue->pLast && QueueValid(pQueue))
    {
    	*pMsgLast = pQueue->pLast;
    	return true;
    }
    return false;
}

bool QueueInsert(Queue * pQueue, void * pMsgPlace, const void * pMsg)
{
    bool bReturn = false;
    if ( pQueue->pLast != NULL && QueueValid(pQueue))
    {
    	if( pQueue->pLast == pMsgPlace )
    	{
    		QueuePush( pQueue, pMsg );
    		bReturn = true;
    	}
    	else if ( pQueue->pFirst > pQueue->pLast )
    	{
    		if (pMsgPlace >= pQueue->pFirst && pMsgPlace <= pQueue->pTail && ((pMsgPlace - pQueue->pFirst) % pQueue->MsgSize == 0))
    		{
        		memmove( pQueue->pFirst - pQueue->MsgSize, pQueue->pFirst, pMsgPlace - pQueue->pFirst + pQueue->MsgSize );
        		memcpy( pMsgPlace, pMsg, pQueue->MsgSize );
        		pQueue->pFirst -= pQueue->MsgSize;
        		if (pQueue->pLast == pQueue->pFirst)
        		{
        			pQueue->pLast = NULL;
        		}
        		bReturn = true;
    		}
    		else if (pMsgPlace >= pQueue->pHead && pMsgPlace < pQueue->pLast && ((pMsgPlace - pQueue->pHead) % pQueue->MsgSize == 0))
    		{
    			memmove( pMsgPlace + pQueue->MsgSize, pMsgPlace, pQueue->pLast - pMsgPlace);
    			memcpy( pMsgPlace, pMsg, pQueue->MsgSize );
    			pQueue->pLast += pQueue->MsgSize;
        		if (pQueue->pLast == pQueue->pFirst)
        		{
        			pQueue->pLast = NULL;
        		}
        		bReturn = true;
    		}
    	}
    	else if ( pQueue->pLast > pQueue->pFirst && pMsgPlace >= pQueue->pFirst && pMsgPlace < pQueue->pLast && (( pMsgPlace - pQueue->pFirst ) % pQueue->MsgSize == 0) )
    	{
    		if ( pQueue->pFirst == pQueue->pHead || pQueue->pLast - pMsgPlace < pMsgPlace - pQueue->pFirst)
    		{
    			memmove( pMsgPlace + pQueue->MsgSize, pMsgPlace, pQueue->pLast - pMsgPlace );
    			memcpy( pMsgPlace, pMsg, pQueue->MsgSize );
    			if( pQueue->pLast >= pQueue->pTail)
    			{
    				 pQueue->pLast = pQueue->pHead;
    			}
    			else
    			{
    				pQueue->pLast += pQueue->MsgSize;
    			}
    			if (pQueue->pLast == pQueue->pFirst)
    			{
    				pQueue->pLast = NULL;
    			}
        		bReturn = true;
    		}
    		else
    		{
        		memmove( pQueue->pFirst - pQueue->MsgSize, pQueue->pFirst, pMsgPlace - pQueue->pFirst + pQueue->MsgSize );
        		memcpy( pMsgPlace, pMsg, pQueue->MsgSize );
        		pQueue->pFirst -= pQueue->MsgSize;
        		bReturn = true;
    		}
    	}
    }
    return bReturn;
}

bool QueueSearchByFunc(const Queue * pQueue, const void * pMsgElement, const void * pMsgMember, bool (*pFunc)(void *), void ** pMsgResult)
{
   	if (pQueue->pFirst != pQueue->pLast)
	{
		uint32_t depth = QueueGetDepth(pQueue);
		if ( depth != -1)
		{
			uint32_t Offset = pMsgMember - pMsgElement;
			void * pWalk;
			for (pWalk = pQueue->pFirst; pWalk != pQueue->pLast ; pWalk != pQueue->pTail ? (pWalk += pQueue->MsgSize) : (pWalk = pQueue->pHead))
			{
				if (pFunc(pWalk + Offset))
				{
					*pMsgResult = pWalk;
					return true;
				}
			}
		}
    }
    return false;
}
