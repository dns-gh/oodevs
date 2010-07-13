// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObstacleAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dico )
    : type_ ( eDemolitionTargetType_Preliminary )
    , bActivated_ ( true )
    , activationTime_ ( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::PropertiesDictionary& dico, Enum_DemolitionTargetType type )
    : type_ ( type )
    , bActivated_ ( type_.GetValue() == eDemolitionTargetType_Preliminary )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : type_ ( attribute( xis, "type", std::string() ) )
    , bActivated_ ( type_.GetValue() == eDemolitionTargetType_Preliminary )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Mine parameters" ) )
             .Display( tools::translate( "Object", "Obstacle type:" ), type_.GetValue() )
             .Display( tools::translate( "Object", "Reserved obstacle activated:" ), bActivated_ );
    if( !bActivated_ )
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ / 3600. * Units::hours );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInTooltip
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Obstacle type:" ), type_.GetValue() );
    if( type_.GetValue() == eDemolitionTargetType_Reserved )
        displayer.Display( tools::translate( "Object", "Reserved obstacle activated:" ), bActivated_ );
    if( !bActivated_ )
        displayer.Display( tools::translate( "Object", "Activation time:" ), activationTime_ / 3600. * Units::hours );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SerializeAttributes
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "obstacle" )
            << attribute( "type", type_.ToXml() )
            << attribute( "activated", bActivated_ )
            << start( "activation-time" )
                << attribute( "value", activationTime_ )
            << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::CreateDictionary
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle type" ), type_ );
    dico.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Obstacle activated" ), bActivated_ );
    dico.Register( *this, tools::translate( "Object", "Info/Demolition target parameters/Activation time" ), activationTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Activate
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObstacleAttribute::Activate( bool activate )
{
    if( type_.GetValue() == eDemolitionTargetType_Reserved )
        bActivated_ = activate;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetActivationTime
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetActivationTime( int time )
{
    activationTime_ = time;
}
