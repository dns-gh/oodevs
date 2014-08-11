// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Agent.h"
#include "tools/IdManager.h"
#include "AgentHierarchies.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AgentType& type, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Agent_ABC >( controller, idManager.GetNextId(), type.GetKeyName().c_str() )
    , type_                ( type )
    , symbolPath_          ( type_.GetSymbol() )
    , moveSymbol_          ( type_.GetMoveSymbol() )
    , staticSymbol_        ( type_.GetStaticSymbol() )
    , level_               ( ENT_Tr::ConvertToNatureLevel( type.GetNature().GetLevel() ) )
    , overridenSymbol_     ( false )
    , nature_              ( type.GetNature().GetNature() )
    , weight_              ( type.GetComposantesWeight() )
{
    AddExtension( *this );
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xml::xistream& xis, Controller& controller, tools::IdManager& idManager, const AgentType& type, const kernel::SymbolFactory& symbolFactory )
    : EntityImplementation< Agent_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_           ( type )
    , symbolPath_     ( type_.GetSymbol() )
    , moveSymbol_          ( type_.GetMoveSymbol() )
    , staticSymbol_        ( type_.GetStaticSymbol() )
    , overridenSymbol_( xis.attribute< bool >( "overridden-symbol", false ) )
    , nature_         ( type.GetNature().GetNature() )
    , weight_         ( type.GetComposantesWeight() )
{
    std::string level = "";
    xis >> xml::optional
            >> xml::attribute( "level", level );
    if( overridenSymbol_ && xis.has_attribute( "nature" ) )
    {
        xis >> xml::attribute( "nature", nature_ );
        if ( nature_.find( "symbols/" ) == std::string::npos )
            symbolPath_ = "symbols/" + nature_;
        else
            symbolPath_ = nature_;
        nature_ = symbolFactory.GetNatureFromSymbol( nature_ );
    }

    level_ = ENT_Tr::ConvertToNatureLevel( ( level.empty() ) ? type.GetNature().GetLevel() : level );
    idManager.Lock( id_ );
    AddExtension( *this );
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeSymbol
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void Agent::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbolPath_ == symbol && levelPath_ == level )
        return;
    symbolPath_ = symbol;
    levelPath_ = level;
    const kernel::Karma& karma = Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    kernel::App6Symbol::SetKarma( symbolPath_, karma );
    kernel::App6Symbol::SetKarma( moveSymbol_, karma );
    kernel::App6Symbol::SetKarma( staticSymbol_, karma );
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && !IsAggregated() )
    {
        InitializeSymbol();

        float width = type_.GetWidth();
        float depth = type_.GetDepth();
        tools.DrawUnitSymbol( symbolPath_, moveSymbol_, staticSymbol_, levelPath_, false, where, -1.f, 0, width, depth );
        tools.DrawApp6SymbolFixedSize( levelPath_, where, -1.f, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Agent::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Type" ), type_, true );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Weight" ), weight_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::SerializeAttributes( xml::xostream& xos ) const
{
    gui::EntityImplementation< kernel::Agent_ABC >::SerializeAttributes( xos );
    xos << xml::attribute( "type", type_.GetKeyName() );
    if( level_ != ENT_Tr::ConvertToNatureLevel( type_.GetNature().GetLevel() ) )
        xos << xml::attribute( "level", ENT_Tr::ConvertFromNatureLevel( level_ ) );
    if( overridenSymbol_ )
    {
        std::string symbol = symbolPath_;
        symbol = symbol.substr( 8, symbol.length() - 8 );
        xos << xml::attribute( "overridden-symbol", true )
            << xml::attribute( "nature", symbol );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::DisplayInTooltip
// Created: LDC 2011-03-11
// -----------------------------------------------------------------------------
void Agent::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    QString id = QString( "[%1]" ).arg( GetId() );
    displayer.Item( "" ).Start( Styles::bold )
                 .Add( (Agent_ABC*)this )
                 .AddToDisplay( id );
    displayer.End();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetLevel
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
E_NatureLevel Agent::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetNature
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
const std::string& Agent::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSymbol
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
const std::string& Agent::GetSymbol() const
{
    return symbolPath_;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetLevel
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void Agent::SetLevel( E_NatureLevel level )
{
    AgentHierarchies& pAgentHierachies = static_cast< AgentHierarchies& >( Get< kernel::TacticalHierarchies >() );
    level_ = level;
    pAgentHierachies.SetLevel( level_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::SetSymbol
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void Agent::SetSymbol( const std::string& symbol )
{
    AgentHierarchies& pAgentHierachies = static_cast< AgentHierarchies& >( Get< kernel::TacticalHierarchies >() );
    overridenSymbol_ = true;
    std::string newSymbol = symbol;
    if ( newSymbol.find( "symbols/" ) == std::string::npos )
        newSymbol = "symbols/" + newSymbol;
    symbolPath_ = newSymbol;
    pAgentHierachies.SetSymbol( newSymbol );
}

// -----------------------------------------------------------------------------
// Name: Agent::SetNature
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void Agent::SetNature( const std::string& nature )
{
    nature_ = nature;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsSymbolOverriden
// Created: JSR 2014-03-11
// -----------------------------------------------------------------------------
bool Agent::IsSymbolOverriden() const
{
    return overridenSymbol_;
}
