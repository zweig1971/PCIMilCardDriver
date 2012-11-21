#ifndef _WINDRVR_INT_THREAD_H_
#define _WINDRVR_INT_THREAD_H_

#include "windrvr.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus 

#if defined(__KERNEL__)

    typedef void (*INT_HANDLER_FUNC)(PVOID pData, PVOID pUserDat);
    typedef struct 
    {
        HANDLE hWD;
        WD_INTERRUPT *pInt;
        INT_HANDLER_FUNC func;
        PVOID pData;
		PVOID pUserDat;
    } INT_THREAD_DATA;

    static void __cdecl InterruptThreadHandler (PVOID pContext)
    {		
		INT_THREAD_DATA *pThread = (INT_THREAD_DATA *) pContext;

        WD_IntCount(pThread->hWD, pThread->pInt);

		pThread->func(pThread->pData);
	}

    static BOOL InterruptThreadEnable(HANDLE *phThread, HANDLE hWD, 
        WD_INTERRUPT *pInt, INT_HANDLER_FUNC func, PVOID pData, PVOID pUserDat)
    {
        INT_THREAD_DATA *pThread;
        *phThread = NULL;

        pThread = (INT_THREAD_DATA *) malloc(sizeof (INT_THREAD_DATA));
        if (!pThread)
            return FALSE;

        pInt->kpCall.hKernelPlugIn = WD_KERNEL_DRIVER_PLUGIN_HANDLE;
        pInt->kpCall.dwMessage = (DWORD) InterruptThreadHandler;
        pInt->kpCall.pData = pThread;
        WD_IntEnable (hWD, pInt);
        // check if WD_IntEnable failed
        if (!pInt->fEnableOk)
            return FALSE;
        
        pThread->pInt = pInt;
        pThread->hWD = hWD;
        pThread->func = func;
        pThread->pData = pData;
		pThread->pUserDat = pUserDat;
		*phThread = (HANDLE) pThread;
        return TRUE;
    }

    static void InterruptThreadDisable(HANDLE hThread)
    {
        if (hThread)
        {
            INT_THREAD_DATA *pThread = (INT_THREAD_DATA *) hThread;
            WD_IntDisable(pThread->hWD, pThread->pInt);
            free (pThread);
        }
    }
#else
    #if !defined(VXWORKS)
        #include <malloc.h>
        #if defined(UNIX)
            #include <pthread.h>
        #endif
    #endif
    
    typedef VOID (*INT_HANDLER_FUNC)(PVOID pData, PVOID pUserDat);
    typedef struct 
    {
        #if !defined(UNIX)
            HANDLE hThread;
        #elif !defined(VXWORKS)
            pthread_t hThread;
        #else
            int hThread;
        #endif
        HANDLE hWD;
        INT_HANDLER_FUNC func;
        PVOID pData;    
        WD_INTERRUPT *pInt;
		PVOID pUserDat;
	} INT_THREAD_DATA;

    #if !defined(UNIX)
    static DWORD WINAPI InterruptThreadHandler (PVOID pData)
    #else
    static void* InterruptThreadHandler (PVOID pData)
    #endif
    {
        INT_THREAD_DATA *pThread = (INT_THREAD_DATA *) pData;
        for (;;)
        {
            WD_IntWait (pThread->hWD, pThread->pInt);
            if (pThread->pInt->fStopped)
                break;
            pThread->func(pThread->pData, pThread->pUserDat);
        }
        return 0;
    }

    static BOOL InterruptThreadEnable(HANDLE *phThread, HANDLE hWD, 
        WD_INTERRUPT *pInt, INT_HANDLER_FUNC func, PVOID pData, PVOID pUserDat, WORD SET_MY_PRIORITY)
    {
        #if defined(VXWORKS)
            int priority;
        #endif

        INT_THREAD_DATA *pThread;
        *phThread = NULL;

        WD_IntEnable (hWD, pInt);
        // check if WD_IntEnable failed
        if (!pInt->fEnableOk)
            return FALSE;
        
        pThread = (INT_THREAD_DATA *) malloc(sizeof (INT_THREAD_DATA));
        pThread->func = func;
        pThread->pData = pData;
        pThread->hWD = hWD;
        pThread->pInt = pInt;
		pThread->pUserDat = pUserDat;
		
        #if !defined(UNIX)
            pThread->hThread = CreateThread (0, 0x1000, 
                InterruptThreadHandler, (PVOID) pThread, 0,&WinDriverGlobalDW);
			SetThreadPriority(pThread->hThread, SET_MY_PRIORITY);
        #elif !defined(VXWORKS)
            pthread_create( &pThread->hThread, NULL, InterruptThreadHandler, 
                (PVOID) pThread);
        #else
            if( taskPriorityGet( taskIdSelf(), &priority ) == ERROR )
            {
                free( pThread );
                return FALSE;
            }

            pThread->hThread = taskSpawn( NULL, priority, 0, 2000, 
                (FUNCPTR)InterruptThreadHandler, (int)pThread, 0, 0, 0, 0, 0,
                0, 0, 0, 0);
            if( pThread->hThread == ERROR )
            {
                free( pThread );
                return FALSE;
            }
        #endif
        *phThread = (HANDLE) pThread;
        return TRUE;
    }

    #if defined(VXWORKS)
    
    #define WAIT_FOR_EVER 0
    
    static void vxTask_wait(int taskId, int waitTime)
    {
        SEM_ID waitSem;
        
        if(waitTime == WAIT_FOR_EVER)
        {
            /* Loop while task is still alive */
            while(taskIdVerify(taskId) == OK)
                taskDelay(3);
        }
        else
        {
            /* 
             * create a dummy semaphore and try to take it for the specified
             * time.
             */ 
            waitSem = semBCreate(SEM_Q_PRIORITY , SEM_EMPTY);
            semTake(waitSem, waitTime);
            semDelete(waitSem);
        }
    }
    #endif
    
    static VOID InterruptThreadDisable(HANDLE hThread)
    {
        if (hThread)
        {
            INT_THREAD_DATA *pThread = (INT_THREAD_DATA *) hThread;
            WD_IntDisable(pThread->hWD, pThread->pInt);
            #if !defined(UNIX)
                WaitForSingleObject (pThread->hThread, INFINITE);
                CloseHandle(pThread->hThread);
            #elif !defined(VXWORKS)
                pthread_join(pThread->hThread, NULL);
            #else
                vxTask_wait((int)pThread->hThread, WAIT_FOR_EVER);
            #endif
            free (pThread);
        }
    }

#endif

#ifdef __cplusplus
}
#endif  // __cplusplus 

#endif

