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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Entity_ABC;
class Model;
class Controller;
class SupplyStates;

// =============================================================================
/** @class  Logistics
    @brief  Logistics
*/
// Created: AGE 2006-03-01
// =============================================================================
class Logistics : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgLogMaintenanceEtat >
                , public Updatable_ABC< ASN1T_MsgLogSanteEtat >
                , public Updatable_ABC< ASN1T_MsgLogRavitaillementEtat >
                , public Updatable_ABC< ASN1T_MsgLogRavitaillementQuotas >
{

public:
    //! @name Constructors/Destructor
    //@{
             Logistics( Entity_ABC& holder, Controller& controller, Model& model );
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
    virtual void DoUpdate( const ASN1T_MsgLogRavitaillementQuotas& message );
    SupplyStates& InstanciateSupplyState();
    //@}

private:
    //! @name Member data
    //@{
    Entity_ABC& holder_;
    Controller& controller_;
    Model& model_;
    //@}
};

#endif // __Logistics_h_
