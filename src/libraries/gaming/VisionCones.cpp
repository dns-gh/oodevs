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
#include "SurfaceFactory_ABC.h"
#include "VisionMap.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Workers.h"
#include "clients_kernel/WorkerTask_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

struct VisionCones::Updater : public kernel::WorkerTask_ABC
{
    Updater( VisionCones& cones )
        : cones_           ( &cones )
        , map_             ( cones.factory_.CreateVisionMap() )
        , cancelled_       ( false )
        , deprecated_      ( false )
        , computed_        ( false )
    {
        cones.current_ = this;
        cones.needUpdating_ = false;
        surfaces_.reserve( cones.surfaces_.size() );
        for( auto it = cones.surfaces_.begin(); it != cones.surfaces_.end(); ++it )
            surfaces_.push_back( new Surface( **it ) );
    }
    virtual ~Updater()
    {
        for( auto it = surfaces_.begin(); it != surfaces_.end(); ++it )
            delete *it;
        delete map_;
    }
    void Cancel()
    {
        cancelled_ = true;
    }
    void Deprecate()
    {
        deprecated_ = true;
    }
    virtual void Process()
    {
        if( cancelled_ || deprecated_ )
            return;
        for( auto it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Initialize( *map_ );
        map_->Initialize();
        for( auto it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Update( *map_ );
        computed_ = true;
    };

    virtual void Commit()
    {
        if( cancelled_ || !computed_ )
            return;
        std::swap( cones_->map_, map_ );
        cones_->needUpdating_ = false;
        cones_->current_      = 0;
    };

private:
    VisionCones* cones_;
    VisionMap* map_;
    T_Surfaces surfaces_;
    bool cancelled_;
    bool deprecated_;
    bool computed_;
};

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones( const Agent_ABC& agent, SurfaceFactory_ABC& factory, Workers& workers, kernel::Controller& controller )
    : agent_           ( agent )
    , factory_         ( factory )
    , workers_         ( workers )
    , controller_      ( controller )
    , map_             ( factory_.CreateVisionMap() )
    , needUpdating_    ( true )
    , current_         ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: VisionCones destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::~VisionCones()
{
    controller_.Unregister( *this );
    CancelCurrent();
    for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
        delete *it;
    delete map_;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::DoUpdate( const sword::UnitVisionCones& message )
{
    for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
        delete *it;
    surfaces_.resize( message.cones().elem_size() );
    for( int i = 0; i < message.cones().elem_size(); ++i )
        surfaces_[i] = factory_.CreateSurface( agent_, message.cones().elem( i ) );
    Invalidate();
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_position()
     || message.has_experience()
     || message.has_stress()
     || message.has_tiredness() )
        Invalidate();
}

// -----------------------------------------------------------------------------
// Name: VisionCones::CancelCurrent
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void VisionCones::CancelCurrent()
{
    if( current_ )
        current_->Cancel();
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Update
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Update() const
{
    // $$$$ SBO 2007-03-20: threaded version disabled
//    Updater updater( *const_cast< VisionCones* >( this ) );
//    updater.Process();
//    updater.Commit();

    // $$$$ AGE 2007-07-09: threaded version enabled
    workers_.Enqueue(
        std::unique_ptr< WorkerTask_ABC >(
            new Updater( *const_cast< VisionCones* >( this ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Draw( const geometry::Point2f& , const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( agent_.IsAggregated() )
        return;
    if( tools.ShouldDisplay( "VisionCones" ) )
        for( auto it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Draw( viewport, tools );
    if( tools.ShouldDisplay( "VisionSurfaces" ) && map_->IsVisible( viewport ) )
    {
        if( needUpdating_ )
        {
            if( current_ )
                current_->Deprecate();
            Update();
        }
        map_->Draw( viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DrawFill
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void VisionCones::DrawFill( const gui::Viewport_ABC& viewport ) const
{
    for( auto it = surfaces_.begin(); it != surfaces_.end(); ++it )
        (*it)->DrawFill( viewport );
}

// -----------------------------------------------------------------------------
// Name: VisionCones::NotifyUpdated
// Created: JSR 2013-02-25
// -----------------------------------------------------------------------------
void VisionCones::NotifyUpdated( const MeteoModel& )
{
    Invalidate();
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Invalidate
// Created: AGE 2006-12-08
// -----------------------------------------------------------------------------
void VisionCones::Invalidate()
{
    needUpdating_ = true;
}
