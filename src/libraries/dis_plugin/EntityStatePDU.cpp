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
#include "dispatcher/Agent.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: EntityStatePDU constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
EntityStatePDU::EntityStatePDU( const dispatcher::Agent& agent, const EntityIdentifier& id, unsigned long time, unsigned char exercise )
    : header_ ( DisHeader::EntityStatePDU( time, exercise ) )
    , id_     ( id )
    , forceID_( 0 )
    , numberOfArticulationParameters_( 0 )
    , entityLinearVelocity_( WorldLocation( agent.position_.strPosition_, (float)agent.nHeight_ ), (float)agent.nSpeed_, (unsigned short)agent.nDirection_  )
    , entityLocation_( agent.position_.strPosition_, (float)agent.nHeight_ )
    , entityOrientation_( entityLocation_, entityLinearVelocity_ )
    , appearance_( 0 )
    , deadReckoningAlgorithm_( 2 )
    , entityMarking_( agent.strName_ )
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
