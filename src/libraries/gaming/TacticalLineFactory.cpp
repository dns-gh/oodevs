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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/Protocol.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory constructor
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
TacticalLineFactory::TacticalLineFactory( kernel::Controllers& controllers,
                                          const kernel::CoordinateConverter_ABC& converter,
                                          Model& model,
                                          Publisher_ABC& publisher,
                                          const kernel::Profile_ABC& profile )
    : controllers_ ( controllers )
    , converter_   ( converter )
    , model_       ( model )
    , publisher_   ( publisher )
    , profile_     ( profile )
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

namespace
{
    void CheckTacticalLineParentExists( const Model& model, const sword::Diffusion& message )
    {
        if( message.has_automat() )
        {
            if( !model.GetAutomatResolver().Find( message.automat().id() ) )
            {
                std::stringstream str;
                str << "Impossible to create limit because parent automat " << message.automat().id() << "does not exist";
                throw std::runtime_error( str.str() );
            }
        }
        else if( message.has_formation() )
        {
            if( !model.GetFormationResolver().Find( message.formation().id() ) )
            {
                std::stringstream str;
                str << "Impossible to create limit because parent formation " << message.formation().id() << "does not exist";
                throw std::runtime_error( str.str() );
            }
        }
        else if( message.has_unit() )
        {
            if( !model.GetAgentResolver().Find( message.unit().id() ) )
            {
                std::stringstream str;
                str << "Impossible to create limit because parent agent " << message.unit().id() << "does not exist";
                throw std::runtime_error( str.str() );
            }
        }
    }
}

namespace
{
    bool CanBeOrdered( const kernel::Entity_ABC& entity, const kernel::Profile_ABC& profile )
    {
        const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
        return superior && profile.CanBeOrdered( *superior );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLineFactory::Create
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
::TacticalLine_ABC* TacticalLineFactory::Create( const sword::PhaseLineCreation& message )
{
    CheckTacticalLineParentExists( model_, message.tactical_line().diffusion() );
    ::TacticalLine_ABC* line = new Lima( controllers_.controller_, publisher_, converter_, message,
                                         [=]( const kernel::TacticalLine_ABC& line ){ return CanBeOrdered( line, profile_ ); } );
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
    CheckTacticalLineParentExists( model_, message.tactical_line().diffusion() );
    ::TacticalLine_ABC* line = new Limit( controllers_.controller_, publisher_, converter_, message,
                                          [=]( const kernel::TacticalLine_ABC& line ){ return CanBeOrdered( line, profile_ ); } );
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
    Limit line( controllers_.controller_, publisher_, converter_,
                [=]( const kernel::TacticalLine_ABC& line ){ return CanBeOrdered( line, profile_ ); } );
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
    Lima line( controllers_.controller_, publisher_, converter_,
               [=]( const kernel::TacticalLine_ABC& line ){ return CanBeOrdered( line, profile_ ); } );
    line.Attach< kernel::Positions >( *new TacticalLinePositions( controllers_.controller_, points, converter_, line ) );
    line.Attach< kernel::TacticalHierarchies >( *new TacticalLineHierarchies( controllers_.controller_, line, superior, model_.GetAutomatResolver(), model_.GetFormationResolver(), model_.GetAgentResolver() ) );
    line.Polish();
    line.Create();
}
