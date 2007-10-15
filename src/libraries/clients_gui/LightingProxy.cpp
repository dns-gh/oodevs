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
#include "graphics/FixedLighting.h"
#include "graphics/TimeLighting.h"

#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
namespace bpt = boost::posix_time;

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LightingProxy constructor
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
LightingProxy::LightingProxy( QObject* parent )
    : QObject( parent )
    , fixed_( new FixedLighting() )
    , time_ ( new TimeLighting() )
{
    current_ = fixed_.get();
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
// Name: LightingProxy::SwitchToSimulationTime
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void LightingProxy::SwitchToSimulationTime()
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
