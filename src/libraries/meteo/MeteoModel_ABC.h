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
#include <boost/noncopyable.hpp>
#include <list>

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
    virtual const PHY_Lighting& GetLighting() const;
    void Purge();
    //@}

protected:
    //! @name Helpers
    //@{
    typedef std::list< PHY_Meteo* >       T_MeteoList;
    typedef T_MeteoList::iterator        IT_MeteoList;
    typedef T_MeteoList::const_iterator CIT_MeteoList;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterMeteo( weather::PHY_Meteo& element );
    virtual void UnregisterMeteo( weather::PHY_Meteo& element );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::CoordinateConverter_ABC& converter_;
    std::auto_ptr< weather::PHY_Meteo > globalMeteo_;
    T_MeteoList meteos_;
    //@}
};

}

#endif // __MeteoModel_ABC_h_
