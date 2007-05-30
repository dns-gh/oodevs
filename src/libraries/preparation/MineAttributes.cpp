// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MineAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: MineAttributes constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttributes::MineAttributes( kernel::PropertiesDictionary& dico )
    : activityTime_( 0, Units::hours )
    , density_     ( 1, Units::minesPerMeter )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttributes::MineAttributes( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : activityTime_( 0, Units::hours )
    , density_     ( 1, Units::minesPerMeter )
{
    std::string density;
    xis >> start( "specific-attributes" )
            >> content( "activity-time", activityTime_.value_ )
            >> content( "density", density )
        >> end();
    density_.value_ = QString( density.c_str() ).toDouble();
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttributes::~MineAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
                .Display( tools::translate( "Object", "Activity time:" ), activityTime_ )
                .Display( tools::translate( "Object", "Density:" ), density_ );
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "activity-time", activityTime_.value_ )
            << content( "density", density_.value_ ) // $$$$ SBO 2007-02-08: oulaylay serialized even for mine jams!
        << end();
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::SetActivityTime
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::SetActivityTime( unsigned int time )
{
    activityTime_.value_ = time;
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::SetDensity
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::SetDensity( float density )
{
    density_.value_ = density;
}

// -----------------------------------------------------------------------------
// Name: MineAttributes::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttributes::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "MineAttributes", "Info/Mine parameters/Activity time" ), activityTime_ );
    dico.Register( *this, tools::translate( "MineAttributes", "Info/Mine parameters/Density" ), density_ );
}