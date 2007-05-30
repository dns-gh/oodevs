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
#include "xeumeuleu/xml.h"

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
    const kernel::AutomatType& ReadType( const kernel::Resolver_ABC< kernel::AutomatType, QString >& types, xml::xistream& input )
    {
        std::string type;
        input >> xml::attribute( "automatType", type );
        return types.Get( QString( type.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const kernel::Resolver_ABC< kernel::AutomatType, QString >& types, xml::xistream& input )
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
    kernel::Formation_ABC* parent = dynamic_cast< kernel::Formation_ABC* >( &superior );
    return parent ? &agents_.CreateAutomat( *parent, type_ ) : 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "automat" )
           << xml::attribute( "automatType", type_.GetName().ascii() );
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AutomatTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return dynamic_cast< const kernel::Formation_ABC* >( &superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString AutomatTemplateElement::GetName() const
{
    return type_.GetName();
}
