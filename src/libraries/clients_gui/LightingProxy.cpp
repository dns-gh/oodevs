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

#include <graphics/FixedLighting.h>
#include <graphics/TimeLighting.h>
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LightingProxy constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
LightingProxy::LightingProxy()
    : camera_( false )
    , fixed_ ( std::make_shared< FixedLighting >() )
    , time_  ( std::make_shared< TimeLighting >() )
    , current_( fixed_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LightingProxy constructor
// Created: ABR 2014-07-04
// -----------------------------------------------------------------------------
LightingProxy::LightingProxy( const LightingProxy& other )
    : camera_( other.camera_ )
    , fixed_ ( std::make_shared< FixedLighting >( *other.fixed_ ) )
    , time_  ( std::make_shared< TimeLighting >( *other.time_ ) )
    , current_( fixed_ )
{
    if( other.current_ == other.time_ )
        current_ = time_;
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
    if( camera_ )
    {
        glPushMatrix();
        glLoadIdentity();
    }
    current_->Set();
    if( camera_ )
        glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SwitchToFixed
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToFixed()
{
    camera_ = false;
    current_ = fixed_;
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SwitchToCameraFixed
// Created: AGE 2008-03-06
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToCameraFixed()
{
    camera_ = true;
    current_ = fixed_;
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SwitchToSimulationTime
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToSimulationTime()
{
    camera_ = false;
    current_ = time_;
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
// Name: LightingProxy::SetLatitude
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void LightingProxy::SetLatitude( float latitude )
{
    time_->SetLatitude( latitude );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::SetTime
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void LightingProxy::SetTime( const QDateTime& time )
{
    time_->SetDayOfYear( time.date().dayOfYear() );
    time_->SetTime     ( time.time().hour() + time.time().minute() / 60.f + time.time().second() / 3600.f );
}

// -----------------------------------------------------------------------------
// Name: LightingProxy::Clone
// Created: ABR 2014-07-04
// -----------------------------------------------------------------------------
std::shared_ptr< Lighting_ABC > LightingProxy::Clone() const
{
    return std::make_shared< LightingProxy >( *this );
}
