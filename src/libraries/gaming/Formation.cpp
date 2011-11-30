// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Formation.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const sword::FormationCreation& message, Controller& controller, const tools::Resolver_ABC< HierarchyLevel_ABC >& resolver )
    : EntityImplementation< Formation_ABC >( controller, message.formation().id(), QString( message.name().c_str() ) )
    , controller_   ( controller )
    , level_        ( resolver.Get( message.level() ) )
    , logisticLevel_( &kernel::LogisticLevel::Resolve( message.logistic_level() ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %2" ).arg( level_.GetName() ).arg( message.formation().id() );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
const kernel::HierarchyLevel_ABC& Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::CreateDictionary
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Formation::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Formation& self = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Identifier" ), self.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Name" ), self.name_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/LogisticLevel" ), *logisticLevel_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLogisticLevel
// Created: AHC 2010-10-08
// -------------------------------------------------------------------------------
const kernel::LogisticLevel& Formation::GetLogisticLevel() const
{
    return *logisticLevel_;
}

// -----------------------------------------------------------------------------
// Name: Formation::InitializeSymbol
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
void Formation::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbolPath_ == symbol && levelPath_ == level )
        return;
    symbolPath_ = symbol;
    levelPath_ = level;
    const Entity_ABC& team = hierarchies.GetTop();
    const Diplomacies_ABC* diplo = team.Retrieve< Diplomacies_ABC >();
    App6Symbol::SetKarma( symbolPath_, diplo ? diplo->GetKarma() : Karma::unknown_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::Draw
// Created: LGY 2011-03-08
// -----------------------------------------------------------------------------
void Formation::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !IsAggregated( *this ) && HasAggregatedSubordinate() && viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbolPath_, where, 4 );
        tools.DrawApp6Symbol( levelPath_, where, 4 );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::IsAggregated
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool Formation::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: Formation::HasAggregatedSubordinate
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool Formation::HasAggregatedSubordinate() const
{
    tools::Iterator< const kernel::Entity_ABC& > it = Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        return IsAggregated( it.NextElement() );
    return false;
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const sword::FormationUpdate& /*message*/ )
{
    controller_.Update( *static_cast< Entity_ABC* >( this ) );
}
