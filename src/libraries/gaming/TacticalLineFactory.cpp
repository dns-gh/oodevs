// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLineFactory.h"
#include "AgentsModel.h"
#include "Lima.h"
#include "Limit.h"
#include "Model.h"
#include "TacticalLinePositions.h"
#include "TacticalLineHierarchies.h"
#include "TeamsModel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controllers.h"
#include "protocol/Protocol.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory constructor
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
TacticalLineFactory::TacticalLineFactory( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, Model& model, Publisher_ABC& publisher )
    : controllers_ ( controllers )
    , converter_   ( converter )
    , model_       ( model )
    , publisher_   ( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory destructor
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
TacticalLineFactory::~TacticalLineFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Create
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
::TacticalLine_ABC* TacticalLineFactory::Create( const sword::PhaseLineCreation& message )
{
    ::TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, converter_, message );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( controllers_.controller_, message.tactical_line().geometry(), converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, *line, message.tactical_line().diffusion(), model_.GetAutomatResolver(), model_.GetFormationResolver(), model_.GetAgentResolver() ) );
    line->Polish();
    return line;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Create
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
::TacticalLine_ABC* TacticalLineFactory::Create( const sword::LimitCreation& message )
{
    ::TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, converter_, message );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( controllers_.controller_, message.tactical_line().geometry(), converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, *line, message.tactical_line().diffusion(), model_.GetAutomatResolver(), model_.GetFormationResolver(), model_.GetAgentResolver() ) );
    line->Polish();
    return line;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::CreateLimit
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::CreateLimit( const T_PointVector& points, const kernel::Entity_ABC& superior )
{
    Limit line( controllers_.controller_, publisher_, converter_ );
    line.Attach< kernel::Positions >( *new TacticalLinePositions( controllers_.controller_, points, converter_, line ) );
    line.Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, line, superior, model_.GetAutomatResolver(), model_.GetFormationResolver(), model_.GetAgentResolver() ) );
    line.Polish();
    line.Create();
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::CreateLima
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::CreateLima( const T_PointVector& points, const kernel::Entity_ABC& superior )
{
    Lima line( controllers_.controller_, publisher_, converter_ );
    line.Attach< kernel::Positions >( *new TacticalLinePositions( controllers_.controller_, points, converter_, line ) );
    line.Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, line, superior, model_.GetAutomatResolver(), model_.GetFormationResolver(), model_.GetAgentResolver() ) );
    line.Polish();
    line.Create();
}
