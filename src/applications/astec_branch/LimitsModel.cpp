// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "Model.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( Model& model, Controllers& controllers )
    : model_( model )
    , controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::~LimitsModel()
{
    Resolver< TacticalLine_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::UpdateToSim
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::UpdateToSim()
{
    for( Resolver< TacticalLine_ABC >::CIT_Elements it = Resolver< TacticalLine_ABC >::elements_.begin(); it != Resolver< TacticalLine_ABC >::elements_.end(); ++it )
    {
        TacticalLine_ABC& line = *it->second;
        line.UpdateToSim();
    }
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::UseSimTacticalLines
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::UseSimTacticalLines()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const ASN1T_MsgLimitCreation& asnMsg )
{
    TacticalLine_ABC* line = new Limit( controllers_.controller_, asnMsg, model_.coordinateConverter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const ASN1T_MsgLimaCreation& asnMsg )
{
    TacticalLine_ABC* line = new Lima( controllers_.controller_, asnMsg, model_.coordinateConverter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLimit
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLimit( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::DeleteLima
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::DeleteLima( unsigned long id )
{
    TacticalLine_ABC* line = Find( id );
    Remove( id );
    delete line;
}

