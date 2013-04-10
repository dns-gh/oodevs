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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Karma.h"
#include "ENT/ENT_Tr_Gen.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Formation::Formation( const sword::FormationCreation& message, Controller& controller )
    : EntityImplementation< Formation_ABC >( controller, message.formation().id(), QString( message.name().c_str() ), true )
    , controller_   ( controller )
    , level_        ( static_cast< E_NatureLevel >( message.level() ) )
    , logisticLevel_( &kernel::LogisticLevel::Resolve( message.logistic_level() ) )
{
    if( name_.isEmpty() )
        name_ = QString( "%1 %L2" ).arg( ENT_Tr::ConvertFromNatureLevel( level_ ).c_str() ).arg( message.formation().id() );
    AddExtension( *this );
    CreateDictionary();
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
E_NatureLevel Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::CreateDictionary
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Formation::CreateDictionary()
{
    PropertiesDictionary& dictionary = Get< PropertiesDictionary >();
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
void Formation::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !IsAggregated( *this ) && HasAggregatedSubordinate() && viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6SymbolFixedSize( symbolPath_, where, -2.f, 0 );
        tools.DrawApp6SymbolFixedSize( levelPath_, where, -2.f, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Formation::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
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
