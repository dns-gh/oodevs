// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "EntityStatePDU.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: EntityStatePDU constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
EntityStatePDU::EntityStatePDU( unsigned long time, unsigned char exercise, const EntityIdentifier& id )
    : header_ ( DisHeader::EntityStatePDU( time, exercise ) )
    , id_     ( id )
    , forceID_( 0 )
    , numberOfArticulationParameters_( 0 )
    , appearance_( 0 )
    , deadReckoningAlgorithm_( 2 )
    , capabilities_( 0 )
{
    entityType_.SetKind( EntityType::platform );
    entityType_.SetDomain( EntityType::land );
    entityType_.SetCountry( EntityType::us );
    entityType_.SetCategory( EntityType::tank );
    alternativeType_ = entityType_;
    ::memset( otherParameters_, 0, 15 );
}

// -----------------------------------------------------------------------------
// Name: EntityStatePDU destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
EntityStatePDU::~EntityStatePDU()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityStatePDU::SetEntityName
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void EntityStatePDU::SetEntityName( const std::string& name )
{
    entityMarking_ = EntityMarking( name );
}

// -----------------------------------------------------------------------------
// Name: EntityStatePDU::SetPosition
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void EntityStatePDU::SetPosition( double latitude, double longitude, float altitude, float speed, float heading )
{
    entityLocation_       = WorldLocation( latitude, longitude, altitude );
    entityLinearVelocity_ = VelocityVector( entityLocation_, speed, heading );
    entityOrientation_    = Orientation( entityLocation_, entityLinearVelocity_ );
}
