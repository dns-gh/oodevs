// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LightingProxy.h"
#include "moc_LightingProxy.cpp"
#include "graphics/FixedLighting.h"
#include "graphics/TimeLighting.h"

#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
namespace bpt = boost::posix_time;

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LightingProxy constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
LightingProxy::LightingProxy( QObject* parent )
    : QObject( parent )
    , fixed_( new FixedLighting() )
    , time_ ( new TimeLighting() )
{
    time_->SetLatitude( latitude_ );
    current_ = fixed_.get();

    QTimer* timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( OnTimer() ) );
    timer->start( 100 );
}
 
// -----------------------------------------------------------------------------
// Name: LightingProxy destructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
LightingProxy::~LightingProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::Set
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::Set()
{
    current_->Set();
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SwitchToFixed
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToFixed()
{
    current_ = fixed_.get();
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SwitchToClockTime
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToClockTime()
{
    current_ = time_.get();
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SetLightDirection
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SetLightDirection( const geometry::Vector3f& direction )
{
    fixed_->SetLightDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SetAmbient
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SetAmbient( float r, float g, float b )
{
    fixed_->SetAmbient( r, g, b );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SetDiffuse
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SetDiffuse( float r, float g, float b )
{
    fixed_->SetDiffuse( r, g, b );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::NotifyUpdated
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::NotifyUpdated( const ModelLoaded& model )
{
    WorldParameters::Load( model.config_ );
    time_->SetLatitude( latitude_ );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::OnTimer
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::OnTimer()
{
    bpt::ptime t = bpt::second_clock::local_time();
    tm pt_tm = bpt::to_tm( t );
    time_->SetDayOfYear( pt_tm.tm_yday );
    time_->SetTime     ( pt_tm.tm_hour + pt_tm.tm_min / 60.f + pt_tm.tm_sec / 3600.f );
}
