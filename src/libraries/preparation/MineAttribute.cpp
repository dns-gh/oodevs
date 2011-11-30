// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MineAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( kernel::PropertiesDictionary& dico )
    : rValorizationPercentage_ ( 0. )
    , nDotationValorization_   ( 0 )
    , valorization_            ( 0 )
    , density_                 ( 1, Units::minesPerMeter )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : rValorizationPercentage_ ( 0. )
    , nDotationValorization_   ( 0 )
    , valorization_            ( 0 )
    , density_     ( 1, Units::minesPerMeter )
{
    std::string density;
    xis >> xml::optional >> xml::content( "density", density );
    density_.value_ = QString( density.c_str() ).toDouble();
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );

    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Development resource:" ) )
                .Start( nDotationValorization_ )
                .Add( " " ).Add( valorization_ ).End();

    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
             .Display( tools::translate( "Object", "Density:" ), density_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void MineAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "mine" )
            << xml::content( "density", density_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::SetDensity
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::SetDensity( float density )
{
    density_.value_ = density;
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "MineAttribute", "Info/Mine parameters/Density" ), density_ );
}