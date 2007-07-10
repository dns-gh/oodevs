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
#include "clients_kernel/WorkerTask_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones( const Agent_ABC& agent, SurfaceFactory& factory, Workers& workers )
    : agent_           ( agent )
    , factory_         ( factory )
    , workers_         ( workers )
    , map_             ( factory_.CreateVisionMap() )
    , needUpdating_    ( true )
    , current_         ( 0 )
    , elongationFactor_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionCones destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::~VisionCones()
{
    CancelCurrent();
    for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    delete map_;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::DoUpdate( const ASN1T_MsgUnitVisionCones& message )
{
    for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    elongationFactor_ = message.elongation;
    
    surfaces_.resize( message.cones.n );
    for( unsigned i = 0; i < message.cones.n; ++i )
        surfaces_[i] = factory_.CreateSurface( agent_, message.cones.elem[i], elongationFactor_ );
    
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

struct VisionCones::Updater : public kernel::WorkerTask_ABC
{
    Updater( VisionCones& cones )
        : cones_           ( &cones )
        , map_             ( cones.factory_.CreateVisionMap() )
        , elongationFactor_( cones.elongationFactor_ )
        , cancelled_       ( false )
        , deprecated_      ( false )
        , computed_        ( false )
    {
        cones.current_ = this;
        surfaces_.reserve( cones.surfaces_.size() );
        for( CIT_Surfaces it = cones.surfaces_.begin(); it != cones.surfaces_.end(); ++it )
            surfaces_.push_back( new Surface( **it ) );
    };
    virtual ~Updater()
    {
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
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
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
            (*it)->Initialize( *map_ );
        map_->Initialize();
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
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
    double elongationFactor_;
    bool cancelled_;
    bool deprecated_;
    bool computed_;
};

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
    std::auto_ptr< WorkerTask_ABC > task( new Updater( *const_cast< VisionCones* >( this ) ) );
    workers_.Enqueue( task );
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void VisionCones::Draw( const geometry::Point2f& , const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "VisionCones" ) )
        for( CIT_Surfaces it = surfaces_.begin(); it != surfaces_.end(); ++it )
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
// Name: VisionCones::Invalidate
// Created: AGE 2006-12-08
// -----------------------------------------------------------------------------
void VisionCones::Invalidate()
{
    needUpdating_ = true;
}
