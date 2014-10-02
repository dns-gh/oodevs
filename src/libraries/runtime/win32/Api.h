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
    virtual std::string       GetLastError         () const;
    virtual bool              CloseHandle          ( void* hObject ) const;
    virtual void*             OpenProcess          ( int dwDesiredAccess, bool bInheritHandle, int dwProcessId ) const;
    virtual int               GetProcessName       ( void* hProcess, wchar_t* lpImageFileName, int nSize ) const;
    virtual int               WaitForSingleObjectEx( void* hHandle, int dwMilliseconds, bool bAlertable ) const;
    virtual bool              TerminateProcess     ( void* hProcess, unsigned uExitCode ) const;
    virtual std::wstring      GetModuleFilename    () const;
    virtual ProcessDescriptor MakeProcess          ( const wchar_t* app, wchar_t* args, const wchar_t* run, const wchar_t* log ) const;
    virtual int               GetCurrentProcessIdentifier() const;
    //@}

private:
    cpplog::BaseLogger& log_;
};
}

#endif
