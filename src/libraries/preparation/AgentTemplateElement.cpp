// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentTemplateElement.h"
#include "AgentsModel.h"

#include "clients_gui/Tools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "preparation/Agent.h"

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents,
                                            const kernel::Agent_ABC& agent )
    : TemplateElement( agent )
    , agents_( agents )
    , type_( agent.GetType() )
    , cp_( tools::IsCommandPost( agent ) )
{
    const Agent& unit = static_cast< const Agent& >( agent );
    level_ = static_cast< int >( unit.GetLevel() );
    if( unit.IsSymbolOverriden() )
        symbol_ = unit.GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents,
                                            const kernel::AgentType& type,
                                            xml::xistream& xis )
    : TemplateElement( xis )
    , agents_( agents )
    , type_( type )
    , cp_( xis.attribute( "commandPost", false ) )
    , level_( xis.attribute( "level", -1 ) )
    , symbol_( xis.attribute( "symbol_", "" ) )
{
    if( name_.isEmpty() )
        name_ = type_.GetLocalizedName().c_str();
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::~AgentTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AgentTemplateElement::Instanciate( kernel::Entity_ABC& superior,
                                                       const geometry::Point2f& center,
                                                       ColorController& colorController )
{
    kernel::Automat_ABC* parent = dynamic_cast< kernel::Automat_ABC* >( &superior );
    if( !parent )
        return 0;
    kernel::Agent_ABC& result = agents_.CreateAgent( *parent, type_, center, cp_, name_ );
    SetColor( result, colorController );
    SetExtensions( result );
    Agent& agent = static_cast< Agent& >( result );
    if( level_ != -1 )
        agent.SetLevel( static_cast< E_NatureLevel >( level_ ) );
    if( !symbol_.empty() )
        agent.SetSymbol( symbol_ );
    return &result;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AgentTemplateElement::Serialize( xml::xostream& xos ) const
{
    TemplateElement::Serialize( xos );
    xos << xml::attribute( "type", "agent" )
        << xml::attribute( "agentType", type_.GetKeyName() )
        << xml::attribute( "commandPost", cp_ );
    if( level_ != -1 )
        xos << xml::attribute( "level", level_ );
    if( !symbol_.empty() )
        xos << xml::attribute( "symbol", symbol_);
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return superior.GetTypeName() == kernel::Automat_ABC::typeName_;
}
