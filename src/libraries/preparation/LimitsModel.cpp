// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LimitsModel.h"
#include "TacticalLine_ABC.h"
#include "Limit.h"
#include "Lima.h"
#include "TacticalLinePositions.h"
#include "TacticalLines.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LimitsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::LimitsModel( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, IdManager& idManager )
    : controllers_( controllers )
    , converter_( converter )
    , idManager_( idManager )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
LimitsModel::~LimitsModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::Purge
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void LimitsModel::Purge()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLimit
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLimit( const T_PointVector& points, kernel::Entity_ABC& superior )
{
    Limit* line = new Limit( controllers_.controller_, idManager_ );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( points, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( *line, &superior ) );
    line->Polish();
    if( TacticalLines* lines = superior.Retrieve< TacticalLines >() )
        lines->AddLine( *line );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLima
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void LimitsModel::CreateLima( const T_PointVector& points, kernel::Entity_ABC& superior )
{
    Lima* line = new Lima( controllers_.controller_, idManager_ );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( points, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( *line, &superior ) );
    line->Polish();
    if( TacticalLines* lines = superior.Retrieve< TacticalLines >() )
        lines->AddLine( *line );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLimit
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void LimitsModel::CreateLimit( xml::xistream& xis, kernel::Entity_ABC& superior )
{
    Limit* line = new Limit( controllers_.controller_, xis, idManager_ );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( xis, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( *line, &superior ) );
    line->Polish();
    if( TacticalLines* lines = superior.Retrieve< TacticalLines >() )
        lines->AddLine( *line );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::CreateLima
// Created: SBO 2006-11-07
// -----------------------------------------------------------------------------
void LimitsModel::CreateLima( xml::xistream& xis, kernel::Entity_ABC& superior )
{
    Lima* line = new Lima( controllers_.controller_, xis, idManager_ );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( xis, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( *line, &superior ) );
    line->Polish();
    if( TacticalLines* lines = superior.Retrieve< TacticalLines >() )
        lines->AddLine( *line );
    Register( line->GetId(), *line );
}

// -----------------------------------------------------------------------------
// Name: LimitsModel::NotifyDeleted
// Created: ABR 2011-10-05
// -----------------------------------------------------------------------------
void LimitsModel::NotifyDeleted( const kernel::TacticalLine_ABC& line )
{
    Remove( line.GetId() );
}
