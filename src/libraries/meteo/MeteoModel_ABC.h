// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __weather_MeteoModel_ABC_h_
#define __weather_MeteoModel_ABC_h_

#include "MeteoManager_ABC.h"
#include <boost/noncopyable.hpp>
#include <set>

namespace sword
{
    class ControlGlobalWeather;
    class ControlLocalWeatherCreation;
    class ControlLocalWeatherDestruction;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace weather
{

// =============================================================================
/** @class  MeteoModel_ABC
    @brief  Meteo model declaration
*/
// Created: HBD 2010-03-23
// =============================================================================
class MeteoModel_ABC : public MeteoManager_ABC
                     , boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MeteoModel_ABC( kernel::CoordinateConverter_ABC& converter );
    virtual ~MeteoModel_ABC();
    //@}

    //! @name Protocol buffer operations
    //@{
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message ) = 0;
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message ) = 0;
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void AddMeteo( weather::Meteo& element );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< weather::Meteo > globalMeteo_;
    //@}
};

}

#endif // __weather_MeteoModel_ABC_h_
