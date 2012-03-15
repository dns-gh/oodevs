// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef API_H__
#define API_H__
#include "Api_ABC.h"

namespace process
{

// =============================================================================
/** @class  Api
    @brief  Win32 API definition
*/
// Created: BAX 2012-03-08
// =============================================================================
class Api : public Api_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Api();
    virtual ~Api();
    //@}

    //! @name Operations
    //@{
    virtual bool                   EnumProcesses        ( DWORD* ids, int cb, DWORD* pBytesReturned ) const;
    virtual bool                   CreateProcess        ( const wchar_t* app, wchar_t* args, SECURITY_ATTRIBUTES* lpProcessAttributes, SECURITY_ATTRIBUTES* lpThreadAttributes, bool bInheritHandles, int dwCreationFlags, void* lpEnvironment, const wchar_t* lpCurrentDirectory, STARTUPINFOW* lpStartupInfo, PROCESS_INFORMATION* lpProcessInformation ) const;
    virtual bool                   CloseHandle          ( HANDLE hObject ) const;
    virtual HANDLE                 OpenProcess          ( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const;
    virtual int                    GetProcessName       ( HANDLE hProcess, wchar_t* lpImageFileName, int nSize ) const;
    virtual int                    WaitForSingleObjectEx( HANDLE hHandle, int dwMilliseconds, bool bAlertable ) const;
    virtual bool                   TerminateProcess     ( HANDLE hProcess, unsigned uExitCode ) const;
    virtual bool                   GetExitCodeProcess   ( HANDLE hProcess, DWORD* lpExitCode ) const;
    virtual HANDLE                 CreateRemoteThreadExt( HANDLE hProcess, SECURITY_ATTRIBUTES* lpThreadAttributes, size_t dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, int dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD* lpThreadId ) const;
    virtual LPTHREAD_START_ROUTINE GetExitProcessPointer() const;
    //@}

private:
    LPTHREAD_START_ROUTINE exit_;
};

}

#endif
