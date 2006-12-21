// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherModel_h_
#define __WeatherModel_h_

#include "clients_kernel/Types.h"
#include "clients_kernel/Resolver.h"
#include <qdatetime.h>

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Weather;
class LocalWeather;

// =============================================================================
/** @class  WeatherModel
    @brief  WeatherModel
*/
// Created: SBO 2006-12-19
// =============================================================================
class WeatherModel : public kernel::Resolver< LocalWeather >
{

public:
    //! @name Constructors/Destructor
    //@{
             WeatherModel( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~WeatherModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load( const std::string& filename );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WeatherModel( const WeatherModel& );            //!< Copy constructor
    WeatherModel& operator=( const WeatherModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadEphemerides  ( xml::xistream& xis );
    void ReadGlobalWeather( xml::xistream& xis );
    void ReadLocalWeather ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}

public:
    //! @name Member data
    QTime                   sunset_;
    QTime                   sunrise_;
    kernel::E_LightingType  lighting_;
    Weather*                globalWeather_;
    //@}
};

#endif // __WeatherModel_h_
