// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logistics_h_
#define __Logistics_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Controller;
    class PropertiesDictionary;
}

class Model;
class StaticModel;
class SupplyStates;

// =============================================================================
/** @class  Logistics
    @brief  Logistics
*/
// Created: AGE 2006-03-01
// =============================================================================
class Logistics : public kernel::Extension_ABC
                , public kernel::Updatable_ABC< ASN1T_MsgLogMaintenanceEtat >
                , public kernel::Updatable_ABC< ASN1T_MsgLogSanteEtat >
                , public kernel::Updatable_ABC< ASN1T_MsgLogRavitaillementEtat >
{

public:
    //! @name Constructors/Destructor
    //@{
             Logistics( kernel::Entity_ABC& holder, kernel::Controller& controller, Model& model, const StaticModel& staticModel, kernel::PropertiesDictionary& dico );
    virtual ~Logistics();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Logistics( const Logistics& );            //!< Copy constructor
    Logistics& operator=( const Logistics& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgLogSanteEtat& message );
    virtual void DoUpdate( const ASN1T_MsgLogMaintenanceEtat& message );
    virtual void DoUpdate( const ASN1T_MsgLogRavitaillementEtat& message );
    SupplyStates& InstanciateSupplyState();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& holder_;
    kernel::Controller& controller_;
    Model& model_;
    const StaticModel& static_;
    kernel::PropertiesDictionary& dico_;
    //@}
};

#endif // __Logistics_h_
