// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoModel_ABC_h_
#define __MeteoModel_ABC_h_

#include "MeteoManager_ABC.h"

namespace sword
{
    class ControlGlobalWeather;
    class ControlLocalWeatherCreation;
    class ControlLocalWeatherDestruction;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             MeteoModel_ABC() {}
    virtual ~MeteoModel_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Lighting& GetLighting() const = 0;
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message ) = 0;
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message ) = 0;
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message ) = 0;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterMeteo  ( PHY_Meteo& weather ) = 0;
    virtual void UnregisterMeteo( PHY_Meteo& weather ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoModel_ABC( const MeteoModel_ABC& );            //!< Copy constructor
    MeteoModel_ABC& operator=( const MeteoModel_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __MeteoModel_ABC_h_
