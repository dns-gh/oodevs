// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef API_ABC_H__
#define API_ABC_H__
#include <boost/noncopyable.hpp>
#define  NOMINMAX
#include <windows.h>

namespace runtime
{

// =============================================================================
/** @class  Api_ABC
    @brief  Win32 API interface definition
*/
// Created: BAX 2012-03-08
// =============================================================================
class Api_ABC : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             Api_ABC() {}
    virtual ~Api_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool                   EnumProcesses        ( DWORD* ids, int cb, DWORD* pBytesReturned ) const = 0;
    virtual bool                   CreateProcess        ( const wchar_t* app, wchar_t* args, SECURITY_ATTRIBUTES* lpProcessAttributes, SECURITY_ATTRIBUTES* lpThreadAttributes, bool bInheritHandles, int dwCreationFlags, void* lpEnvironment, const wchar_t* lpCurrentDirectory, STARTUPINFOW* lpStartupInfo, PROCESS_INFORMATION* lpProcessInformation ) const = 0;
    virtual bool                   CloseHandle          ( HANDLE hObject ) const = 0;
    virtual HANDLE                 OpenProcess          ( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const = 0;
    virtual int                    GetProcessName       ( HANDLE hProcess, wchar_t* lpImageFileName, int nSize ) const = 0;
    virtual int                    WaitForSingleObjectEx( HANDLE hHandle, int dwMilliseconds, bool bAlertable ) const = 0;
    virtual bool                   TerminateProcess     ( HANDLE hProcess, unsigned uExitCode ) const = 0;
    virtual bool                   GetExitCodeProcess   ( HANDLE hProcess, DWORD* lpExitCode ) const = 0;
    virtual HANDLE                 CreateRemoteThreadExt( HANDLE hProcess, SECURITY_ATTRIBUTES* lpThreadAttributes, size_t dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, int dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD* lpThreadId ) const = 0;
    virtual LPTHREAD_START_ROUTINE GetExitProcessPointer() const = 0;
    //@}
};

}

#endif
