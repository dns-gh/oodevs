// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************


#include "preparation_pch.h"
#include "ObjectAttributesContainer.h"
#include "clients_kernel/Displayer_ABC.h"

#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::ObjectAttributesContainer
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
ObjectAttributesContainer::ObjectAttributesContainer()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::~AttributeContainer
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
ObjectAttributesContainer::~ObjectAttributesContainer()
{
    // NOTHING
}

namespace 
{
    tools::SortedInterfaceContainer< Extension_ABC >& Interface( const ObjectAttributesContainer& rhs )
    {
        return *const_cast< ObjectAttributesContainer* >( &rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::SerializeAttributes
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
void ObjectAttributesContainer::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "attributes" );
        Interface( *this ).Apply( & Serializable_ABC::SerializeAttributes, xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::Display
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
void ObjectAttributesContainer::Display( Displayer_ABC& displayer ) const
{
    Interface( *this ).Apply( & Displayable_ABC::Display, displayer );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::DisplayInList
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
void ObjectAttributesContainer::DisplayInList( Displayer_ABC& displayer ) const
{
    Interface( *this ).Apply( & Displayable_ABC::DisplayInList, displayer );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::DisplayInTooltip
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
void ObjectAttributesContainer::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Interface( *this ).Apply( & Displayable_ABC::DisplayInTooltip, displayer );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributesContainer::DisplayInSummary
// Created: JCR 2008-07-09
// -----------------------------------------------------------------------------
void ObjectAttributesContainer::DisplayInSummary( Displayer_ABC& displayer ) const
{
     Interface( *this ).Apply( & Displayable_ABC::DisplayInSummary, displayer );
}
