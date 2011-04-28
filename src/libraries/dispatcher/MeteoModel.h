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
             MeteoModel( const Config& config, Model& model );
    virtual ~MeteoModel();
    //@}

    //! @name Operations
    //@{
    virtual const weather::PHY_Lighting& GetLighting() const;
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message );
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message );
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message );
    void Accept( kernel::ModelVisitor_ABC& visitor );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterMeteo  ( weather::PHY_Meteo& weather );
    virtual void UnregisterMeteo( weather::PHY_Meteo& weather );
    //@}

private:
    //! @name Types
    //@{
   typedef std::list< weather::PHY_Meteo* > T_MeteoList;
   typedef T_MeteoList::iterator          CIT_MeteoList;
    //@}

private:
    //! @name Member data
    //@{
    Model&              model_;
    const Config&       config_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    weather::PHY_Meteo* pGlobalMeteo_;
    T_MeteoList         meteos_;    // Including global meteo
    //@}
};
}

#endif // __MeteoModel_h_
