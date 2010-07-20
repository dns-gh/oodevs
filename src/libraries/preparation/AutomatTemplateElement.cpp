// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatTemplateElement.h"
#include "AgentsModel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AutomatType.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const kernel::Automat_ABC& automat )
    : agents_( agents )
    , type_  ( automat.GetType() )
    , name_  ( automat.GetName() )
{
    // NOTHING
}

namespace
{
    const kernel::AutomatType& ReadType( const tools::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input )
    {
        return types.Get( input.attribute< std::string >( "automatType" ) );
    }

    void ReadName( xml::xistream& input, QString& name )
    {
        std::string str;
        input >> xml::optional >> xml::attribute( "name", str );
        name = str.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const tools::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input )
    : agents_( agents )
    , type_  ( ReadType( types, input ) )
{
    ReadName( input, name_ );
    if( name_.isEmpty() )
        name_ = type_.GetName().c_str();
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::~AutomatTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AutomatTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& )
{
    if( dynamic_cast< kernel::Formation_ABC* >( &superior )
     || dynamic_cast< kernel::Automat_ABC* >( &superior ) )
        return &agents_.CreateAutomat( superior, type_, name_ );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "automat" )
           << xml::attribute( "automatType", type_.GetName() )
           << xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AutomatTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return dynamic_cast< const kernel::Formation_ABC* >( &superior )
         || dynamic_cast< const kernel::Automat_ABC* >( &superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString AutomatTemplateElement::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Rename
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Rename( const QString& name )
{
    name_ = name;
}
