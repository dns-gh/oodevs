// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherModel_h_
#define __WeatherModel_h_

#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgStartFireEffect;
    class MsgStopFireEffect;
}

namespace kernel
{
    class Controllers;
}

class AmmoEffect;
class Model;

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
             WeatherModel( kernel::Controllers& controllers, Model& model );
    virtual ~WeatherModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateAmmoEffect( const MsgsSimToClient::MsgStartFireEffect& message );
    void DeleteAmmoEffect( const MsgsSimToClient::MsgStopFireEffect& message );
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
    Model& model_;
    //@}
};

#endif // __WeatherModel_h_
