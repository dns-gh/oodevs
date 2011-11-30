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
#include "IdManager.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
Agent::Agent( const AgentType& type, Controller& controller, IdManager& idManager )
    : EntityImplementation< Agent_ABC >( controller, idManager.GetNextId(), type.GetName().c_str() )
    , type_                ( type )
    , symbol_              ( type_.GetSymbol() )
    , criticalIntelligence_( "" )
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
Agent::Agent( xml::xistream& xis, Controller& controller, IdManager& idManager, const AgentType& type )
    : EntityImplementation< Agent_ABC >( controller, xis.attribute< unsigned long >( "id" ), ReadName( xis ) )
    , type_       ( type )
    , symbol_     ( type_.GetSymbol() )
{
    std::string criticalIntelligence;
    xis >> xml::optional
            >> xml::start( "critical-intelligence" )
                >> xml::attribute( "content", criticalIntelligence )
            >> xml::end;
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

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, -1.f );
        tools.DrawApp6Symbol( type_.GetLevelSymbol(), where, -1.f );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Agent::InitializeSymbol() const
{
    kernel::App6Symbol::SetKarma( symbol_, Get< CommunicationHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
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
// Name: Agent::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Agent::Rename( const QString& name )
{
    name_ = name;
    Touch();
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Name" ), name_, *this, &Agent::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Type" ), constSelf.type_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Agent", "Info/Critical intelligence" ), criticalIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Agent::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "type", type_.GetName() )
        << xml::attribute( "name", name_.ascii() );
    if( criticalIntelligence_ != "" )
    {
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_.ascii() )
            << xml::end;
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
