// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoModel_h_
#define __MeteoModel_h_

#include "meteo/MeteoModel_ABC.h"
#include <list>

namespace kernel
{
    class CoordinateConverter_ABC;
    class CoordinateConverter;
    class Controller;
}

class Simulation;

// =============================================================================
/** @class  MeteoModel
    @brief  MeteoModel
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoModel : public weather::MeteoModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MeteoModel( kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, kernel::Controller& controller );
    virtual ~MeteoModel();
    //@}

    //! @name Accessors
    //@{
    virtual const weather::Meteo* GetGlobalMeteo() const;
    const weather::Meteo* GetMeteo( const geometry::Point2f& point ) const;
    const T_Meteos& GetLocalMeteos() const;
    virtual void Purge();
    //@}

    //! @name Protocol buffer operations
    //@{
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message );
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message );
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Simulation&   simulation_;
    //@}
};

#endif // __MeteoModel_h_
