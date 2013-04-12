// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesGroup.h"
#include <boost/foreach.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesGroup constructor
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
PropertiesGroup::PropertiesGroup( Displayer_ABC& comparator, const Entity_ABC& owner )
    : comparator_( comparator )
    , owner_( owner )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup destructor
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
PropertiesGroup::~PropertiesGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::CreateEditor
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
QWidget* PropertiesGroup::CreateEditor( QWidget* parent, EditorFactory_ABC& factory )
{
    return subProperties_.front()->CreateEditor( parent, factory );;
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::SetValueFromEditor
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
void PropertiesGroup::SetValueFromEditor( QWidget* editor )
{
    BOOST_FOREACH( Property_ABC* property, subProperties_ )
        property->SetValueFromEditor( editor );
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::Display
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
void PropertiesGroup::Display( Displayer_ABC& displayer )
{
    comparator_.Clear();
    if( HasSameProperty() )
        subProperties_.front()->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::GetName
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
const QString& PropertiesGroup::GetName() const
{
    return subProperties_.front()->GetName();
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::GetCategory
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
E_Category PropertiesGroup::GetCategory() const
{
    return subProperties_.front()->GetCategory();
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::AddSubProperty
// Created: LGY 2013-01-15
// -----------------------------------------------------------------------------
void PropertiesGroup::AddSubProperty( Property_ABC* property )
{
    subProperties_.push_back( property );
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::HasSameProperty
// Created: LGY 2013-01-16
// -----------------------------------------------------------------------------
bool PropertiesGroup::HasSameProperty() const
{
    BOOST_FOREACH( Property_ABC* property, subProperties_ )
        if( property )
            property->Display( comparator_ );
    return comparator_.IsValid();
}

// -----------------------------------------------------------------------------
// Name: PropertiesGroup::PropertiesGroup
// Created: MMC 2013-03-07
// -----------------------------------------------------------------------------
const Entity_ABC& PropertiesGroup:: GetOwner() const
{
    return owner_;
}
