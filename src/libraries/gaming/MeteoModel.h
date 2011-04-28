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
#include "protocol/Protocol.h"
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
    virtual const weather::PHY_Lighting& GetLighting() const;
    virtual const weather::PHY_Meteo* GetGlobalMeteo() const;
    //@}

    //! @name Operations
    //@{
    virtual void OnReceiveMsgGlobalMeteo( const sword::ControlGlobalWeather& message );
    virtual void OnReceiveMsgLocalMeteoDestruction( const sword::ControlLocalWeatherDestruction& message );
    virtual void OnReceiveMsgLocalMeteoCreation( const sword::ControlLocalWeatherCreation& message );
    const weather::PHY_Meteo* GetMeteo( const geometry::Point2f& point ) const;
    void Purge();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void RegisterMeteo( weather::PHY_Meteo& element );
    virtual void UnregisterMeteo( weather::PHY_Meteo& element );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoModel( const MeteoModel& );            //!< Copy constructor
    MeteoModel& operator=( const MeteoModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::list< weather::PHY_Meteo* > T_MeteoList;
    typedef T_MeteoList::const_iterator    CIT_MeteoList;
    typedef T_MeteoList::iterator           IT_MeteoList;
    //@}

private:
    //! @name Member data
    //@{
    T_MeteoList                         meteos_;
    kernel::CoordinateConverter_ABC&    converter_;
    kernel::Controller&                 controller_;
    std::auto_ptr< weather::PHY_Meteo > pGlobalMeteo_;
    const Simulation&                   simulation_;
    //@}
};

#endif // __MeteoModel_h_
