
/*
32-Bit Processes (and 16-Bit Processes under Windows 95) Under Win32, the 
operating system promises to clean up resources owned by a process when it shuts 
down. This does not, however, mean that the process itself has had the 
opportunity to do any final flushes of information to disk, any final 
communication over a remote connection, nor does it mean that the process' DLL's 
will have the opportunity to execute their PROCESS_DETACH code. This is why it 
is generally preferable to avoid terminating an application under Windows 95 and 
Windows NT. 

If you absolutely must shut down a process, follow these steps: Post a WM_CLOSE 
to all Top-Level windows owned by the process that you want to shut down. Many 
Windows applications respond to this message by shutting down. NOTE: A console 
application's response to WM_CLOSE depends on whether or not it has installed a 
control handler. For additional information, please see the following article in 
the Microsoft Knowledge Base: Q102429 HOWTO: Detect Closure of Command Window 
from a Console App Use EnumWindows() to find the handles to your target windows. 
In your callback function, check to see if the windows' process ID matches the 
process you want to shut down. You can do this by calling 
GetWindowThreadProcessId(). Once you have established a match, use PostMessage() 
or SendMessageTimeout() to post the WM_CLOSE message to the window. 


Use WaitForSingleObject() to wait for the handle of the process. Make sure you 
wait with a timeout value, because there are many situations in which the 
WM_CLOSE will not shut down the application. Remember to make the timeout long 
enough (either with WaitForSingleObject(), or with SendMessageTimeout()) so that 
a user can respond to any dialog boxes that were created in response to the 
WM_CLOSE message. 


If the return value is WAIT_OBJECT_0, then the application closed itself down 
cleanly. If the return value is WAIT_TIMEOUT, then you must use 
TerminateProcess() to shutdown the application. NOTE: If you are getting a 
return value from WaitForSingleObject() other then WAIT_OBJECT_0 or 
WAIT_TIMEOUT, use GetLastError() to determine the cause. 


By following these steps, you give the application the best possible chance to 
shutdown cleanly (aside from IPC or user-intervention). The 16-Bit Issue (under 
Windows NT) The preceding steps work for 16-bit applications under Windows 95, 
however, Windows NT 16-bit applications work very differently. 

Under Windows NT, all 16-bit applications run in a virtual DOS machine (VDM). 
This VDM runs as a Win32 process (NTVDM) under Windows NT. The NTVDM process has 
a process ID. You can obtain a handle to the process through OpenProcess(), just 
like you can with any other Win32 process. Nevertheless, none of the 16-bit 
applications running in the VDM have a process ID, and therefore you cannot get 
a Process Handle from OpenProcess(). Each 16-bit application in a VDM has a 
16-bit Task Handle and a 32-bit thread of execution. The handle and thread ID 
can be found through a call to the function VDMEnumTaskWOWEx(). For additional 
information, please see the following article in the Microsoft Knowledge Base: 
Q175030 HOWTO: Enumerate Applications in Win32 Your first, and most 
straightforward, option when shutting down a 16-bit application under Windows NT 
is to shut down the entire NTVDM process. You can do this by following the steps 
outlined above. You only need to know the process ID of the NTVDM process (see 
the KB article Q175030 cited above to find the process ID of an NTVDM). The 
downside of this approach is that it closes all 16-bit applications that are 
running in that VDM. If this is not your goal, then you need to take another 
approach. 

If you wish to shut down a single 16-bit application within a NTVDM process, 
following are the steps you need to take: Post a WM_CLOSE to all Top-Level 
windows that are owned by the process, and that have the same owning thread ID 
as the 16-bit task you want to shut down. The most effective way to do this is 
by using EnumWindows(). In your callback function, check to see if the window's 
process ID and thread ID matches the 16-bit task you want to shut down. Remember 
that the process ID is going to be the process ID of the NTVDM process in which 
the 16-bit application is running. 


Although you have a thread ID, you have no way to wait on the termination of the 
16-bit process. As a result, you must wait for an arbitrary length of time (to 
allow a clean shut down), and then try to shut the application down anyway. If 
the application has already shut down, then this will do nothing. If it hasn't 
shut down, then it will terminate the application. 


Terminate the application using a function called VDMTerminateTaskWOW(), which 
can be found in the Vdmdbg.dll. It takes the process ID of the VDM and the task 
number of the 16-bit task. 


This approach allows you to shut down a single 16-bit application within a VDM 
under Windows NT. However, 16-bit Windows is not very good at cleaning up 
resources of a terminated task, and neither is the WOWExec running in the VDM. 
If you are looking for the cleanest possible approach to terminating a 16-bit 
application under Windows NT, you should consider terminating the entire VDM 
process. NOTE: If you are starting a 16-bit application that you may terminate 
later, then use the CREATE_SEPARATE_WOW_VDM with CreateProcess().

*/

#include <windows.h>

#define TA_FAILED 0
#define TA_SUCCESS_CLEAN 1
#define TA_SUCCESS_KILL 2
#define TA_SUCCESS_16 3

DWORD WINAPI Terminate32App( DWORD dwPID, DWORD dwTimeout, DWORD bKillItIfNoRespond );
DWORD WINAPI Terminate16App( DWORD dwPID, DWORD dwThread, WORD w16Task, DWORD dwTimeout );

//******************
//Source
//******************

//#include "TermApp.h"
#include <vdmdbg.h>

typedef struct
{
  DWORD   dwID ;
  DWORD   dwThread ;
} TERMINFO ;

// Declare Callback Enum Functions.
BOOL CALLBACK Terminate32AppEnum( HWND hwnd, LPARAM lParam ) ;

BOOL CALLBACK Terminate16AppEnum( HWND hwnd, LPARAM lParam ) ;

/*----------------------------------------------------------------
DWORD WINAPI Terminate32App( DWORD dwPID, DWORD dwTimeout )

Purpose:
  Shut down a 32-Bit Process (or 16-bit process under Windows 95)

Parameters:
  dwPID
     Process ID of the process to shut down.

  dwTimeout
     Wait time in milliseconds before shutting down the process.

Return Value:
  TA_FAILED - If the shutdown failed.
  TA_SUCCESS_CLEAN - If the process was shutdown using WM_CLOSE.
  TA_SUCCESS_KILL - if the process was shut down with
     TerminateProcess().
  NOTE:  See header for these defines.
----------------------------------------------------------------*/ 
DWORD WINAPI Terminate32App( DWORD dwPID, DWORD dwTimeout, DWORD bKillItIfNoRespond )
{
  HANDLE   hProc ;
  DWORD   dwRet ;

  // If we can't open the process with PROCESS_TERMINATE rights,
  // then we give up immediately.
  hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
     dwPID);

  if(hProc == NULL)
  {
     return TA_FAILED ;
  }

  // Terminate32AppEnum() posts WM_CLOSE to all windows whose PID
  // matches your process's.
  EnumWindows((WNDENUMPROC)Terminate32AppEnum, (LPARAM) dwPID) ;

  // Wait on the handle. If it signals, great. If it times out,
  // then you kill it.
  if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
  {
      if( bKillItIfNoRespond )
      {
        dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
      }
      else
      {
        dwRet = TA_FAILED;
      }
  }
  else
  {
     dwRet = TA_SUCCESS_CLEAN;
  }

  CloseHandle(hProc) ;

  return dwRet ;
}

/*----------------------------------------------------------------
DWORD WINAPI Terminate16App( DWORD dwPID, DWORD dwThread,
                    WORD w16Task, DWORD dwTimeout )

Purpose:
  Shut down a Win16 APP.

Parameters:
  dwPID
     Process ID of the NTVDM in which the 16-bit application is
     running.

  dwThread
     Thread ID of the thread of execution for the 16-bit
     application.

  w16Task
     16-bit task handle for the application.

  dwTimeout
     Wait time in milliseconds before shutting down the task.

Return Value:
  If successful, returns TA_SUCCESS_16
  If unsuccessful, returns TA_FAILED.
  NOTE:  These values are defined in the header for this
  function.

NOTE:
  You can get the Win16 task and thread ID through the
  VDMEnumTaskWOW() or the VDMEnumTaskWOWEx() functions.
----------------------------------------------------------------*/ 
DWORD WINAPI Terminate16App( DWORD dwPID, DWORD dwThread,
                    WORD w16Task, DWORD dwTimeout )
{
  HINSTANCE      hInstLib ;
  TERMINFO      info ;

  // You will be calling the functions through explicit linking
  // so that this code will be binary compatible across
  // Win32 platforms.
  BOOL (WINAPI *lpfVDMTerminateTaskWOW)(DWORD dwProcessId,
     WORD htask) ;

  hInstLib = LoadLibraryA( "VDMDBG.DLL" ) ;
  if( hInstLib == NULL )
     return TA_FAILED ;

  // Get procedure addresses.
  lpfVDMTerminateTaskWOW = (BOOL (WINAPI *)(DWORD, WORD ))
     GetProcAddress( hInstLib, "VDMTerminateTaskWOW" ) ;

  if( lpfVDMTerminateTaskWOW == NULL )
  {
     FreeLibrary( hInstLib ) ;
     return TA_FAILED ;
  }

  // Post a WM_CLOSE to all windows that match the ID and the
  // thread.
  info.dwID = dwPID ;
  info.dwThread = dwThread ;
  EnumWindows((WNDENUMPROC)Terminate16AppEnum, (LPARAM) &info) ;

  // Wait.
  Sleep( dwTimeout ) ;

  // Then terminate.
  lpfVDMTerminateTaskWOW(dwPID, w16Task) ;

  FreeLibrary( hInstLib ) ;
  return TA_SUCCESS_16 ;
}

BOOL CALLBACK Terminate32AppEnum( HWND hwnd, LPARAM lParam )
{
  DWORD dwID ;

  GetWindowThreadProcessId(hwnd, &dwID) ;

  if(dwID == (DWORD)lParam)
  {
     PostMessage(hwnd, WM_CLOSE, 0, 0) ;
  }

  return TRUE ;
}

BOOL CALLBACK Terminate16AppEnum( HWND hwnd, LPARAM lParam )
{
  DWORD      dwID ;
  DWORD      dwThread ;
  TERMINFO   *termInfo ;

  termInfo = (TERMINFO *)lParam ;

  dwThread = GetWindowThreadProcessId(hwnd, &dwID) ;

  if(dwID == termInfo->dwID && termInfo->dwThread == dwThread )
  {
     PostMessage(hwnd, WM_CLOSE, 0, 0) ;
  }

  return TRUE ;
} 
