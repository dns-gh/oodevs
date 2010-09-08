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
#include "clients_kernel/PropertiesDictionary.h"
#include "tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute constructor
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
StructuralStateAttribute::StructuralStateAttribute( unsigned int value, kernel::PropertiesDictionary& dico )
    : structuralState_( value )
{
    CreateDictionary( dico );
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
// Name: StructuralStateAttribute::CreateDictionary
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "StructuralStateAttribute", "Info/StructuralState" ), structuralState_ );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SerializeAttributes
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "structural" )
            << xml::attribute( "value", structuralState_ )
        << xml::end;
}
