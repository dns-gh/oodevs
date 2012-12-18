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
#include "clients_kernel/AgentNature.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ENT/ENT_Tr_Gen.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AgentType& type, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Agent_ABC >( controller, idManager.GetNextId(), type.GetName().c_str() )
    , type_                ( type )
    , symbolPath_          ( type_.GetSymbol() )
    , criticalIntelligence_( "" )
    , level_               ( ENT_Tr::ConvertToNatureLevel( type.GetNature().GetLevel() ) )
    , overridenSymbol_     ( false )
    , nature_              ( type.GetNature().GetNature() )
    , weight_              ( type.GetComposantesWeight() )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Agent::Agent( xml::xistream& xis, Controller& controller, tools::IdManager& idManager, const AgentType& type, const kernel::SymbolFactory& symbolFactory )
    : EntityImplementation< Agent_ABC >( controller, xis.attribute< unsigned long >( "id" ), ReadName( xis ) )
    , type_           ( type )
    , symbolPath_     ( type_.GetSymbol() )
    , overridenSymbol_( xis.attribute< bool >( "overridden-symbol", false ) )
    , nature_         ( type.GetNature().GetNature() )
    , weight_         ( type.GetComposantesWeight() )
{
    std::string criticalIntelligence = "";
    std::string level = "";
    xis >> xml::optional
            >> xml::start( "critical-intelligence" )
                >> xml::attribute( "content", criticalIntelligence )
            >> xml::end
        >> xml::optional
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
    criticalIntelligence_ = criticalIntelligence.c_str();
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    Destroy();
}

namespace
{
    // $$$$ LGY 2012-02-07 : hardcoded for displaying !!!
    float GetFactor( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return 5.f;
        else if( karma == kernel::Karma::enemy_ )
            return 16.f;
        else if( karma == kernel::Karma::neutral_ )
            return 7.f;
        return 0.f;
    }
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
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        const kernel::Karma& karma = Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
        kernel::App6Symbol::SetKarma( symbolPath_, karma );
        tools.DrawApp6Symbol( symbolPath_, where, -1.f );
        geometry::Point2f center( where.X(), where.Y() + GetFactor( karma ) );
        tools.DrawApp6Symbol( levelPath_, center, -1.f );
    }
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
void Agent::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Agent& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Name" ), name_, *static_cast< kernel::EntityImplementation< kernel::Agent_ABC >* >( this ), &Agent::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Type" ), constSelf.type_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Critical intelligence" ), criticalIntelligence_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Weight" ), weight_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "type", type_.GetName() )
        << xml::attribute( "name", name_.toAscii().constData() );
    if( level_ != ENT_Tr::ConvertToNatureLevel( type_.GetNature().GetLevel() ) )
        xos << xml::attribute( "level", ENT_Tr::ConvertFromNatureLevel( level_ ) );
    if( overridenSymbol_ )
    {
        std::string symbol = symbolPath_;
        symbol = symbol.substr( 8, symbol.length() - 8 );
        xos << xml::attribute( "overridden-symbol", true )
            << xml::attribute( "nature", symbol );
    }
    if( criticalIntelligence_() != "" )
    {
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_().toAscii().constData() )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::DisplayInTooltip
// Created: LDC 2011-03-11
// -----------------------------------------------------------------------------
void Agent::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    QString id = QString( "[%L1]" ).arg( GetId() );
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
