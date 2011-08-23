// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FireAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/FireClass.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( kernel::PropertiesDictionary& dico )
    : fireClass_( 0 )
    , maxCombustionEnergy_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::FireClass, std::string >& FireClasses, kernel::PropertiesDictionary& dico )
{
    std::string className;
    xis >> xml::attribute( "class", className )
        >> xml::attribute( "max-combustion-energy", maxCombustionEnergy_ );
    fireClass_ = FireClasses.Find( className );
    if( !fireClass_ )
        xis.error( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void FireAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Fire" ) )
        .Display( tools::translate( "Object", "Fire class:" ), fireClass_ )
        .Display( tools::translate( "Object", "Max combustion energy:" ), maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SetAgent
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void FireAttribute::SetClass( const kernel::FireClass& fireClass )
{
    fireClass_ = const_cast< kernel::FireClass* >( &fireClass );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SetMaxCombustionEnergy
// Created: BCI 2010-12-07
// -----------------------------------------------------------------------------
void FireAttribute::SetMaxCombustionEnergy( int m )
{
    maxCombustionEnergy_ = m;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void FireAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "fire" )
            << xml::attribute( "class", fireClass_->GetName() )
            << xml::attribute( "max-combustion-energy", maxCombustionEnergy_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void FireAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "FireAttribute", "Info/Fire attributes/Fire class" ), fireClass_ );
    dico.Register( *this, tools::translate( "FireAttribute", "Info/Fire attributes/Max combustion energy" ), maxCombustionEnergy_ );
}
