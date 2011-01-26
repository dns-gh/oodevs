// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Weather_h_
#define __Weather_h_

#include <map>
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

namespace sword
{
    class ControlGlobalWeather;
}

class CWISEDriver;
class CWISEValueUnion;
class SwordMessagePublisher_ABC;

// =============================================================================
/** @class  Weather
    @brief  Weather
*/
// Created: SEB 2011-01-07
// =============================================================================
class Weather
{

public:
    //! @name Constructors/Destructor
    //@{
             Weather();
    virtual ~Weather();
    //@}

    //! @name Operations
    //@{
    void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlGlobalWeather& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlGlobalWeather& message );
    void Destroy( CWISEDriver& driver, const WISE_HANDLE& database );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Weather( const Weather& );            //!< Copy constructor
    Weather& operator=( const Weather& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    WISE_HANDLE handle_;
    std::map< std::wstring, WISE_HANDLE > attributes_;
    //@}
};

#endif // __Weather_h_
