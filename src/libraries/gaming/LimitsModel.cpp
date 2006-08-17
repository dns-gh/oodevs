// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "clients_kernel/Controllers.h"
#include "Publisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( Controllers& controllers, const CoordinateConverter_ABC& converter, Publisher_ABC& publisher )
    : controllers_( controllers )
    , converter_( converter )
    , publisher_( publisher )
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
    TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, asnMsg, converter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Create
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LimitsModel::Create( const ASN1T_MsgLimaCreation& asnMsg )
{
    TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, asnMsg, converter_ );
    Register( asnMsg.oid, *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLimit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLimit( const T_PointVector& points )
{
    TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, points, converter_ );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLima
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLima( E_FuncLimaType type, const T_PointVector& points )
{
    TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, points, type, converter_ );
    Register( line->GetId(), *line );
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

