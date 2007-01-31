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

#include "game_asn/Asn.h"
#include "clients_kernel/Resolver.h"

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
class WeatherModel : public kernel::Resolver< AmmoEffect >
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

    void CreateAmmoEffect( const ASN1T_MsgStartFireEffect& message );
    void DeleteAmmoEffect( const ASN1T_MsgStopFireEffect& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WeatherModel( const WeatherModel& );            //!< Copy constructor
    WeatherModel& operator=( const WeatherModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    //@}
};

#endif // __WeatherModel_h_
