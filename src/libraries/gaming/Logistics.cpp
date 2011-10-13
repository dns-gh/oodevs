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

// -----------------------------------------------------------------------------
// Name: Logistics constructor
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
Logistics::Logistics( kernel::Entity_ABC& holder, kernel::Controller& controller, Model& model, const StaticModel& staticModel, kernel::PropertiesDictionary& dico )
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
void Logistics::DoUpdate( const sword::LogMaintenanceState& message )
{
    if( ! holder_.Retrieve< MaintenanceStates >() )
    {
        MaintenanceStates* ext = new MaintenanceStates( controller_, static_.objectTypes_, model_.GetAutomatResolver(), dico_ );
        holder_.Attach( *ext );
        ext->DoUpdate( message );
    }
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const sword::LogMedicalState& message )
{
    if( ! holder_.Retrieve< MedicalStates >() )
    {
        MedicalStates* ext = new MedicalStates( controller_, static_.objectTypes_, model_.GetAutomatResolver(), dico_);
        holder_.Attach( *ext );
        ext->DoUpdate( message );
    }
}

// -----------------------------------------------------------------------------
// Name: Logistics::DoUpdate
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void Logistics::DoUpdate( const sword::LogSupplyState& message )
{
    if( ! holder_.Retrieve< SupplyStates >() )
    {
        SupplyStates* ext = new SupplyStates( controller_, static_.objectTypes_, static_.objectTypes_, dico_);
        holder_.Attach( *ext );
        ext->DoUpdate( message );
    }
}

