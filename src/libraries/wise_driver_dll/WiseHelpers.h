// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Helpers_h_
#define __Helpers_h_

#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace wise
{
    template< typename T >
    void FetchEventAttribute( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle, const std::wstring& name, T& result )
    {
        try
        {
            WISE_HANDLE attribute = WISE_INVALID_HANDLE;
            CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeHandle( database, handle, name, attribute ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeValue( database, handle, attribute, result ) );
        }
        catch( WISE_RESULT& error )
        {
            std::wstringstream ss;
            ss << L"Failed to retrieve event '" << handle << "' attribute '" << name << L"'";
            driver.NotifyErrorMessage( ss.str(), error );
            throw;
        }
    }

    template< typename T >
    void FetchObjectAttribute( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle, const std::wstring& name, T& result )
    {
        try
        {
            WISE_HANDLE attribute = WISE_INVALID_HANDLE;
            CHECK_WISE_RESULT_EX( driver.GetSink()->GetAttributeHandle( database, handle, name, attribute ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->GetAttributeValue( database, handle, attribute, result ) );
        }
        catch( WISE_RESULT& error )
        {
            std::wstringstream ss;
            ss << L"Failed to retrieve object '" << handle << "' attribute '" << name << L"'";
            driver.NotifyErrorMessage( ss.str(), error );
            throw;
        }
    }
}

#endif // __Helpers_h_
