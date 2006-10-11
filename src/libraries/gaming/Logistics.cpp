// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Logistics.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "SupplyStates.h"
#include "clients_kernel/Entity_ABC.h"
#include "Model.h"
#include "clients_kernel/ObjectTypes.h"
#include "AgentsModel.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Logistics constructor
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
Logistics::Logistics( Entity_ABC& holder, Controller& controller, Model& model, const StaticModel& staticModel, DataDictionary& dico )
    : holder_    ( holder )
    , controller_( controller )
    , model_     ( model )
    , static_    ( staticModel )
    , dico_      ( dico )
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
        MaintenanceStates* ext = new MaintenanceStates( controller_, static_.objectTypes_, model_.agents_, dico_ );
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
        MedicalStates* ext = new MedicalStates( controller_, static_.objectTypes_, model_.agents_, dico_);
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
    SupplyStates* ext = new SupplyStates( controller_, static_.objectTypes_, static_.objectTypes_, dico_);
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
