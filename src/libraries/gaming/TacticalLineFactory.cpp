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
#include "TacticalLine_ABC.h"
#include "Lima.h"
#include "Limit.h"
#include "Publisher_ABC.h"
#include "Model.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "TacticalLinePositions.h"
#include "TacticalLineHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory constructor
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
TacticalLineFactory::TacticalLineFactory( Controllers& controllers, const CoordinateConverter_ABC& converter, Model& model, Publisher_ABC& publisher )
    : controllers_ ( controllers )
    , converter_   ( converter )
    , model_       ( model )
    , publisher_   ( publisher )
    , selectedSuperior_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory destructor
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
TacticalLineFactory::~TacticalLineFactory()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Create
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
::TacticalLine_ABC* TacticalLineFactory::Create( const ASN1T_MsgLimaCreation& asnMsg )
{
    ::TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, asnMsg );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( asnMsg.tactical_line.geometrie, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, *line, asnMsg.tactical_line.diffusion, model_.agents_, model_.teams_ ) );
    return line;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Create
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
::TacticalLine_ABC* TacticalLineFactory::Create( const ASN1T_MsgLimitCreation& asnMsg )
{
    ::TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, asnMsg );
    line->Attach< kernel::Positions >( *new TacticalLinePositions( asnMsg.tactical_line.geometrie, converter_, *line ) );
    line->Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, *line, asnMsg.tactical_line.diffusion, model_.agents_, model_.teams_ ) );
    return line;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::CreateLimit
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::CreateLimit( const T_PointVector& points )
{
    Limit line( controllers_.controller_, publisher_ );
    line.Attach< kernel::Positions >( *new TacticalLinePositions( points, converter_, line ) );
    line.Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, line, *selectedSuperior_, model_.agents_, model_.teams_ ) );
    line.Create();
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::CreateLima
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::CreateLima( const T_PointVector& points )
{
    Lima line( controllers_.controller_, publisher_ );
    line.Attach< kernel::Positions >( *new TacticalLinePositions( points, converter_, line ) );
    line.Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, line, *selectedSuperior_, model_.agents_, model_.teams_ ) );
    line.Create(); 
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::BeforeSelection
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::BeforeSelection()
{
    selectedSuperior_ = 0;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::AfterSelection
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::AfterSelection()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Select
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::Select( const kernel::Automat_ABC& element )
{
    selectedSuperior_ = &element;
}
    
// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Select
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void TacticalLineFactory::Select( const kernel::Formation_ABC& element )
{
    selectedSuperior_ = &element;
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::IsReady
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
bool TacticalLineFactory::IsReady() const
{
    return selectedSuperior_;
}
