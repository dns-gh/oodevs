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
#include "clients_kernel/Tools.h"
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

namespace
{
    struct StructuralSetter
    {
        void operator()( unsigned int* pValue, unsigned int value )
        {
            *pValue = std::min( 100u, std::max( 0u, value ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::CreateDictionary
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "StructuralStateAttribute", "Info/StructuralState" ), structuralState_, StructuralSetter() );
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SerializeAttributes
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    if( structuralState_ != 100 )
    {
        xos << xml::start( "structural-state" )
            << xml::attribute( "value", structuralState_ )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::SetOverriden
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void StructuralStateAttribute::SetOverriden( bool& overriden ) const
{
    if( structuralState_ != 100 )
        overriden = true;
}

// -----------------------------------------------------------------------------
// Name: StructuralStateAttribute::Update
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void StructuralStateAttribute::Update( xml::xistream& xis )
{
    xis >> xml::attribute( "value", structuralState_ );
}
