// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Logistics.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "SupplyStates.h"
#include "Entity_ABC.h"
#include "Model.h"
#include "ObjectTypes.h"
#include "AgentsModel.h"

// -----------------------------------------------------------------------------
// Name: Logistics constructor
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
Logistics::Logistics( Entity_ABC& holder, Controller& controller, Model& model )
    : holder_    ( holder )
    , controller_( controller )
    , model_     ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logistics destructor
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
Logistics::~Logistics()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const ASN1T_MsgLogMaintenanceEtat& message )
{
    if( ! holder_.Retrieve< MaintenanceStates >() )
    {
        MaintenanceStates* ext = new MaintenanceStates( controller_, model_.objectTypes_, model_.agents_ );
        holder_.Attach( *ext );
        ext->DoUpdate( message );
    }
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const ASN1T_MsgLogSanteEtat& message )
{
    if( ! holder_.Retrieve< MedicalStates >() )
    {
        MedicalStates* ext = new MedicalStates( controller_, model_.objectTypes_, model_.agents_ );
        holder_.Attach( *ext );
        ext->DoUpdate( message );
    }
}

// -----------------------------------------------------------------------------
// Name: Logistics::InstanciateSupplyState
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
SupplyStates& Logistics::InstanciateSupplyState()
{
    SupplyStates* ext = new SupplyStates( controller_, model_.objectTypes_, model_.objectTypes_ );
    holder_.Attach( *ext );
    return *ext;
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const ASN1T_MsgLogRavitaillementEtat& message )
{
    if( ! holder_.Retrieve< SupplyStates >() )
        InstanciateSupplyState().DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const ASN1T_MsgLogRavitaillementQuotas& message )
{
    if( ! holder_.Retrieve< SupplyStates >() )
        InstanciateSupplyState().DoUpdate( message );
}
