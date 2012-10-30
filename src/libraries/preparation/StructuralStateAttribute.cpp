// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "StructuralStateAttribute.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2012-09-05
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( xml::xistream& xis, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
    : structuralState_( 100, kernel::Units::percentage )
{
    xis >> xml::optional >> xml::start( "structural-state" )
            >> xml::attribute( "value", structuralState_.value_ )
        >> xml::end;
    CreateDictionary( object, dictionary );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
    : structuralState_( 100, kernel::Units::percentage )
{
    CreateDictionary( object, dictionary );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute destructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::~StructuralStateAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SerializeAttributes
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    SerializeObjectAttributes( xos );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SerializeObjectAttributes
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    if( structuralState_.value_ != 100 )
        xos << xml::start( "structural-state" )
                << xml::attribute( "value", structuralState_.value_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::Update
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void StructuralStateAttribute::Update( xml::xistream& xis )
{
    xis >> xml::attribute( "value", structuralState_.value_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::GetValue
// Created: LGY 2012-06-15
// -----------------------------------------------------------------------------
unsigned int StructuralStateAttribute::GetValue() const
{
    return structuralState_.value_;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::CreateDictionary
// Created: JSR 2012-09-05
// -----------------------------------------------------------------------------
void StructuralStateAttribute::CreateDictionary( kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dictionary )
{
    dictionary.Register( object, tools::translate( "StructuralStateAttribute", "Info/StructuralState" ), structuralState_ );
}
