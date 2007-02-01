// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionCones.h"
#include "Surface.h"
#include "clients_kernel/GlTools_ABC.h"
#include "SurfaceFactory.h"
#include "VisionMap.h"
#include "clients_kernel/Workers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones( const Agent_ABC& agent, SurfaceFactory& factory, Workers& workers )
    : agent_( agent )
    , factory_( factory )
    , workers_( workers )
    , map_( factory_.CreateVisionMap() )
    , needUpdating_( true )
    , updating_    ( false )
{
    // NOTHING
}

struct VisionCones::NotUpdating
{
    NotUpdating( VisionCones* cones ) : cones_( cones ) {};
    bool operator()() const { return ! cones_->updating_; }
    VisionCones* cones_;
};

// -----------------------------------------------------------------------------
// Name: VisionCones destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::~VisionCones()
{
    boost::mutex::scoped_lock locker( mutex_ );
    condition_.wait( locker, NotUpdating( this ) );

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
    boost::mutex::scoped_lock locker( mutex_ );

    for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    surfaces_.clear();
    unsigned long nNbrSurfaces;
    message >> nNbrSurfaces;
    surfaces_.reserve( nNbrSurfaces );
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_.push_back( factory_.CreateSurface( agent_, message ) );
    message >> elongationFactor_;
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_[ i ]->SetElongation( elongationFactor_ );
    Invalidate();
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
        Invalidate();
}

struct VisionCones::Updater
{
    Updater( VisionCones& cones )
        : cones_           ( &cones )
        , map_             ( cones.factory_.CreateVisionMap() )
        , elongationFactor_( cones.elongationFactor_ )
    {
        cones.updating_ = true;
        surfaces_.reserve( cones.surfaces_.size() );
        for( CIT_Surfaces it = cones.surfaces_.begin(); it != cones.surfaces_.end(); ++it )
            surfaces_.push_back( new Surface( **it ) );
    };

    void operator()()
    {
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Initialize( *map_ );
        map_->Initialize();
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Update( *map_ );
        Commit();
    };

    void Commit()
    {
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            delete *it;

        boost::mutex::scoped_lock locker( cones_->mutex_ );

        std::swap( cones_->map_, map_ );
        delete map_;
        cones_->updating_     = false;
        cones_->needUpdating_ = false;
        cones_->condition_.notify_one();
    };
private:
    VisionCones* cones_;
    VisionMap* map_;
    T_Surfaces surfaces_;
    double elongationFactor_;
};

// -----------------------------------------------------------------------------
// Name: VisionCones::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Update() const
{
    workers_.Enqueue( Updater( *const_cast< VisionCones* >( this ) ) );
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Draw( const geometry::Point2f& , const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    boost::mutex::scoped_lock locker( mutex_ );
     
    if( tools.ShouldDisplay( "VisionCones" ) )
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Draw( viewport, tools );

    if( tools.ShouldDisplay( "VisionSurfaces" ) )
    {
        if( needUpdating_ && ! updating_ )
            Update();
        map_->Draw( viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Invalidate
// Created: AGE 2006-12-08
// -----------------------------------------------------------------------------
void VisionCones::Invalidate()
{
    needUpdating_ = true;
}
