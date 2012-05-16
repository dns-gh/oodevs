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

namespace cpplog
{
    class BaseLogger;
};

namespace runtime
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
             Api( cpplog::BaseLogger& log );
    virtual ~Api();
    //@}

    //! @name Operations
    //@{
    virtual std::string            GetLastError         () const;
    virtual bool                   EnumProcesses        ( DWORD* ids, int cb, DWORD* pBytesReturned ) const;
    virtual bool                   CloseHandle          ( HANDLE hObject ) const;
    virtual HANDLE                 OpenProcess          ( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const;
    virtual int                    GetProcessName       ( HANDLE hProcess, wchar_t* lpImageFileName, int nSize ) const;
    virtual int                    WaitForSingleObjectEx( HANDLE hHandle, int dwMilliseconds, bool bAlertable ) const;
    virtual bool                   TerminateProcess     ( HANDLE hProcess, unsigned uExitCode ) const;
    virtual bool                   GetExitCodeProcess   ( HANDLE hProcess, DWORD* lpExitCode ) const;
    virtual HANDLE                 CreateRemoteThreadExt( HANDLE hProcess, SECURITY_ATTRIBUTES* lpThreadAttributes, size_t dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, int dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD* lpThreadId ) const;
    virtual LPTHREAD_START_ROUTINE GetExitProcessPointer() const;
    virtual std::wstring           GetModuleFilename    () const;
    virtual ProcessDescriptor      MakeProcess          ( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log ) const;
    //@}

private:
    mutable cpplog::BaseLogger& log_;
    LPTHREAD_START_ROUTINE exit_;
};

}

#endif
