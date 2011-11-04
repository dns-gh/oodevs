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
#include "CommandPostAttributes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const CommandPostAttributes* pAttributes = entity.Retrieve< CommandPostAttributes >() )
            return pAttributes->IsCommandPost();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents, const kernel::Agent_ABC& agent )
    : agents_( agents )
    , type_  ( agent.GetType() )
    , cp_    ( IsCommandPost( agent ) )
    , name_  ( agent.GetName() )
{
    // NOTHING
}

namespace
{
    const kernel::AgentType& ReadType( const tools::Resolver_ABC< kernel::AgentType, std::string >& types, xml::xistream& input )
    {
        return types.Get( input.attribute< std::string >( "agentType" ) );
    }

    void ReadName( xml::xistream& input, QString& name )
    {
        std::string str;
        input >> xml::optional >> xml::attribute( "name", str );
        name = str.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents, const tools::Resolver_ABC< kernel::AgentType, std::string >& types, xml::xistream& input )
    : agents_( agents )
    , type_  ( ReadType( types, input ) )
{
    ReadName( input, name_ );
    if( name_.isEmpty() )
        name_ = type_.GetName().c_str();
    input >> xml::attribute( "commandPost", cp_ );
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
kernel::Entity_ABC* AgentTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center )
{
    kernel::Automat_ABC* parent = dynamic_cast< kernel::Automat_ABC* >( &superior );
    return parent ? & agents_.CreateAgent( *parent, type_, center, cp_, name_ ) : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AgentTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "agent" )
           << xml::attribute( "agentType", type_.GetName() )
           << xml::attribute( "commandPost", cp_ )
           << xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return dynamic_cast< const kernel::Automat_ABC* >( &superior ) != 0;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString AgentTemplateElement::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Rename
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
void AgentTemplateElement::Rename( const QString& name )
{
    name_ = name;
}
