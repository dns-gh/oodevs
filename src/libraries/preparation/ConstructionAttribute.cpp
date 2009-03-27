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
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( kernel::PropertiesDictionary& dico )
    : rConstructionPercentage_ ( 0., Units::percentage )    
    , nDotationConstruction_   ( 0 )
    , construction_            ( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( xistream& /*xis*/, kernel::PropertiesDictionary& dico )
    : rConstructionPercentage_ ( 0., Units::percentage )    
    , nDotationConstruction_   ( 0 )
    , construction_            ( 0 )
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
             .Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ );

//    displayer.Group( tools::translate( "Object", "Information" ) )
//             .Item( tools::translate( "Object", "Construction dotation:" ) )
//                .Start( nDotationConstruction_ )
//                .Add( " " ).Add( construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ConstructionAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::SerializeAttributes( xml::xostream& /*xos*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ConstructionAttribute::CreateDictionary( kernel::PropertiesDictionary& /*dico*/ )
{
    // NOTHING
}