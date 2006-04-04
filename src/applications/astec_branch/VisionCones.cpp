// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "VisionCones.h"
#include "Surface.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones( const Agent& agent, const Resolver_ABC< SensorType, std::string >& resolver )
    : agent_( agent )
    , resolver_( resolver )
    , needsUpdating_( false )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: VisionCones destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::~VisionCones()
{
     for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::DoUpdate( const VisionConesMessage& message )
{
    for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    surfaces_.clear();
    unsigned long nNbrSurfaces;
    message >> nNbrSurfaces;
    surfaces_.reserve( nNbrSurfaces );
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_.push_back( new Surface( message, resolver_ ) );

    message >> elongationFactor_;
    needsUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent 
     || message.m.experiencePresent 
     || message.m.fatiguePresent )
        needsUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Draw( const geometry::Point2f& , const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "VisionCones" ) )
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Draw( agent_, tools );
}
