// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "ApplicationMutex.h"

#include <windows.h>

using namespace tools ; 

class ApplicationMutexInternal
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ApplicationMutexInternal( const std::string& name ); 
    virtual ~ApplicationMutexInternal();
    //@}

    bool IsOwned() { return ( handle_ != NULL ); };  

private:
    //! @name members
    //@{
    HANDLE handle_ ; 
    //@}
}; 

ApplicationMutexInternal::ApplicationMutexInternal( const std::string& name )
{
    handle_ = ::CreateMutex( NULL, false, name.c_str() ); 
    DWORD err = GetLastError();
    if( err == ERROR_ALREADY_EXISTS ) 
        handle_ = NULL ; 
}

// -----------------------------------------------------------------------------
// Name: ApplicationMutex::~ApplicationMutexInternal
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
ApplicationMutexInternal::~ApplicationMutexInternal()
{
    if ( handle_ ) 
        ::ReleaseMutex( handle_ ) ; 
}


// -----------------------------------------------------------------------------
// Name: ApplicationMutex constructor
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
ApplicationMutex::ApplicationMutex( const std::string& name )
    : internal_( new ApplicationMutexInternal(name) ) 
{

}

// -----------------------------------------------------------------------------
// Name: ApplicationMutex::IsOwned
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
bool ApplicationMutex::IsOwned()
{
    return internal_->IsOwned()  ; 
}

// -----------------------------------------------------------------------------
// Name: ApplicationMutex destructor
// Created: RDS 2008-08-18
// -----------------------------------------------------------------------------
ApplicationMutex::~ApplicationMutex()
{
    // NOTHING
}
