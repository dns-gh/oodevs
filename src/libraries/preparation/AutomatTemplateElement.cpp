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
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const kernel::Automat_ABC& automat )
    : agents_( agents )
    , type_  ( automat.GetType() )
{
    // NOTHING
}

namespace
{
    const kernel::AutomatType& ReadType( const tools::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input )
    {
        return types.Get( xml::attribute< std::string >( input, "automatType" ) );
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
    // NOTHING
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
        return &agents_.CreateAutomat( superior, type_ );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "automat" )
           << xml::attribute( "automatType", type_.GetName() );
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
    return type_.GetName().c_str();
}
