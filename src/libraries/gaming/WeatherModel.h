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

#include "tools/Resolver.h"

namespace sword
{
    class StartFireEffect;
    class StopFireEffect;
}

namespace kernel
{
    class Controllers;
}

class AmmoEffect;
class Model;
class Simulation;

// =============================================================================
/** @class  WeatherModel
    @brief  WeatherModel
*/
// Created: AGE 2006-04-04
// =============================================================================
class WeatherModel : public tools::Resolver< AmmoEffect >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherModel( kernel::Controllers& controllers, Model& model, const Simulation& simulation );
    virtual ~WeatherModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateAmmoEffect( const sword::StartFireEffect& message );
    void DeleteAmmoEffect( const sword::StopFireEffect& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherModel( const WeatherModel& );            //!< Copy constructor
    WeatherModel& operator=( const WeatherModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model&               model_;
    const Simulation&    simulation_;
    //@}
};

#endif // __WeatherModel_h_
