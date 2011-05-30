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
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model;
    class Config;

// =============================================================================
/** @class  MeteoModel
    @brief  MeteoModel
*/
// Created: HBD 2010-03-23
// =============================================================================
class MeteoModel : public weather::MeteoModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MeteoModel( kernel::CoordinateConverter_ABC& converter, const Config& config, Model& model );
    virtual ~MeteoModel();
    //@}

    //! @name Operations
    //@{
    void Accept( kernel::ModelVisitor_ABC& visitor );
    //@}

    //! @name Protocol buffer operations
    //@{
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message );
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message );
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    Model&        model_;
    const Config& config_;
    //@}
};
}

#endif // __MeteoModel_h_
