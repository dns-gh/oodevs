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
#include <string>

namespace runtime
{
// =============================================================================
/** @class  ProcessDescriptor
    @brief  ProcessDescriptor struct definition
*/
// Created: BAX 2012-05-16
// =============================================================================
struct ProcessDescriptor
{
    ProcessDescriptor() : handle( 0 ), pid( 0 )
    {
        // NOTHING
    }
    ProcessDescriptor( void* handle, int pid ) : handle( handle ), pid( pid )
    {
        // NOTHING
    }
    void* handle;
    int pid;
};

// =============================================================================
/** @class  Api_ABC
    @brief  Win32 API interface definition
*/
// Created: BAX 2012-03-08
// =============================================================================
struct Api_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
    //@{
             Api_ABC() {}
    virtual ~Api_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string       GetLastError         () const = 0;
    virtual bool              CloseHandle          ( void* hObject ) const = 0;
    virtual void*             OpenProcess          ( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const = 0;
    virtual int               GetProcessName       ( void* hProcess, wchar_t* lpImageFileName, int nSize ) const = 0;
    virtual int               WaitForSingleObjectEx( void* hHandle, int dwMilliseconds, bool bAlertable ) const = 0;
    virtual bool              TerminateProcess     ( void* hProcess, unsigned uExitCode ) const = 0;
    virtual std::wstring      GetModuleFilename    () const = 0;
    virtual ProcessDescriptor MakeProcess          ( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log ) const = 0;
    virtual int               GetCurrentProcessIdentifier() const = 0;
    static  bool              Has64BitSystem       ();
    static  bool              Is64BitBinary        ( const wchar_t* app );
    //@}
};
}

#endif
