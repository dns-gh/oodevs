// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ConstructionAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( kernel::PropertiesDictionary& dico )
    : completion_( 0, kernel::Units::percentage )
    , dotationType_( 0 )
    , dotationCount_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : completion_( unsigned int( xis.attribute< float >( "completion", 1.f ) * 100 ), kernel::Units::percentage )
    , dotationType_( 0 )
    , dotationCount_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Construction:" ), completion_ );

//    displayer.Group( tools::translate( "Object", "Information" ) )
//             .Item( tools::translate( "Object", "Construction resource:" ) )
//                .Start( nDotationConstruction_ )
//                .Add( " " ).Add( construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ConstructionAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Construction:" ), completion_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "construction" )
            << xml::attribute( "completion", float( completion_.value_ ) / 100.f );
    if( dotationType_ != 0 && dotationCount_ != 0 )
        xos << xml::start( "resources" )
                << xml::start( "dotation" )
                    << xml::attribute( "name", dotationType_->GetName() )
                    << xml::attribute( "count", dotationCount_ )
                << xml::end
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Object", "Info/Construction/Completion" ), completion_ );
    dico.Register( *this, tools::translate( "Object", "Info/Construction/Resource/Type" ), dotationType_ );
    dico.Register( *this, tools::translate( "Object", "Info/Construction/Resource/Count" ), dotationCount_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SetCompletion
// Created: SBO 2009-06-09
// -----------------------------------------------------------------------------
void ConstructionAttribute::SetCompletion( unsigned int value )
{
    completion_.value_ = value;
}
