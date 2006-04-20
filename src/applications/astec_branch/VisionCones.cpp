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
#include "SurfaceFactory.h"
#include "VisionMap.h"
#include "Workers.h"

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones( const Agent& agent, SurfaceFactory& factory, Workers& workers )
    : agent_( agent )
    , factory_( factory )
    , workers_( workers )
    , map_( factory_.CreateVisionMap() )
    , needUpdating_( true )
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
     delete map_;
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
        surfaces_.push_back( factory_.CreateSurface( agent_, message ) );

    message >> elongationFactor_; // $$$$ AGE 2006-04-04: wtf ?
    needUpdating_ = true;
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
        needUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Updater
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
VisionCones::Updater::Updater( const VisionCones& cones )
    : cones_( & cones )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: VisionCones::operator()
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void VisionCones::Updater::operator()()
{
    cones_->updating_ = true;
    cones_->map_->Clear();
    for( CIT_Surfaces it = cones_->surfaces_.begin(); it != cones_->surfaces_.end(); ++it )
        (*it)->Update( *cones_->map_ );
    cones_->needUpdating_ = false;
    cones_->updating_ = false;
    // $$$$ AGE 2006-04-20: add a ninja kung fu grip ?
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Update() const
{
    workers_.Enqueue( Updater( *this ) );
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Draw( const geometry::Point2f& , const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "VisionCones" ) )
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Draw( tools );
    if( tools.ShouldDisplay( "VisionSurfaces" ) )
    {
        if( needUpdating_ )
            Update();
        else if( ! updating_ )
            map_->Draw( tools );
    }
}
