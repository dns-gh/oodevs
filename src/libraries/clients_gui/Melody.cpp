// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Melody.h"
#include "tools/win32/XPSLights.h"
#include "tools/thread/Thread.h"

#pragma warning ( disable : 4244 )

// -----------------------------------------------------------------------------
// Name: Melody constructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Melody::Melody()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Melody destructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Melody::~Melody()
{
    // NOTHING
}

namespace
{
    Melody BuildExplosion()
    {
        Melody result;
        result.StartSequence();
            result.SetLights( XPSLights::ruby, XPSLights::ruby, XPSLights::ruby, 7 );
            result.Pause( 20 );
            result.SetLights( XPSLights::citrine, XPSLights::citrine, XPSLights::citrine, 7 );
            result.Pause( 20 );
        result.Repeat( 10 );
        result.SetLights( XPSLights::emerald, XPSLights::emerald, XPSLights::emerald, 4 );
        return result;
    };

    Melody BuildTickStart()
    {
        Melody result;
        result.SetBrightness( 7 );
        result.Pause( 20 );
        result.SetBrightness( 4 );
        result.Pause( 20 );
        return result;
    };
};

// -----------------------------------------------------------------------------
// Name: Melody::Explosion
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Melody Melody::Explosion()
{
    static Melody explosion = BuildExplosion();
    return explosion;
}

// -----------------------------------------------------------------------------
// Name: Melody::TickStart
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Melody Melody::TickStart()
{
    static Melody tickstart = BuildTickStart();
    return tickstart;
}

namespace
{
    enum
    {
        side, front, top, brightness, pause, all, repeat, start
    };
}

// -----------------------------------------------------------------------------
// Name: Melody::StartSequence
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::StartSequence()
{
    AddOperation( start, 0 );
}

// -----------------------------------------------------------------------------
// Name: Melody::Repeat
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::Repeat( short times )
{
    short i = operations_.size() - 1;
    while( i >= 0 && operations_.at( i ).type != start )
        --i;
    if( i < 0 )
        throw std::runtime_error( "No start sequence called" );
    AddOperation( repeat, ( i << 16 ) | times );
}

// -----------------------------------------------------------------------------
// Name: Melody::SetLights
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::SetLights( char side, char front, char top, char brightness )
{
    AddOperation( all, (side << 24) | (front << 16) | (top << 8 ) | brightness );
}

// -----------------------------------------------------------------------------
// Name: Melody::SetSideColor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::SetSideColor( char value )
{
    AddOperation( side, value );
}

// -----------------------------------------------------------------------------
// Name: Melody::SetFrontColor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::SetFrontColor( char value )
{
    AddOperation( front, value );
}

// -----------------------------------------------------------------------------
// Name: Melody::SetTopColor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::SetTopColor( char value )
{
    AddOperation( top, value );
}

// -----------------------------------------------------------------------------
// Name: Melody::SetBrightness
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::SetBrightness( char value )
{
    AddOperation( brightness, value );
}


// -----------------------------------------------------------------------------
// Name: Melody::Pause
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::Pause( unsigned msec )
{
    AddOperation( pause, msec );
}

// -----------------------------------------------------------------------------
// Name: Melody::Play
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::Play( XPSLights& lights ) const
{
    for( CIT_Operations it = operations_.begin(); it != operations_.end(); ++it )
        PlayOperation( *it, lights );
}

// -----------------------------------------------------------------------------
// Name: Melody::AddOperation
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::AddOperation( char type, unsigned parameter )
{
    operations_.push_back( Operation() );
    operations_.back().type  = type;
    operations_.back().value = parameter;
}

// -----------------------------------------------------------------------------
// Name: Melody::PlayOperation
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::PlayOperation( const Operation& op, XPSLights& lights ) const
{
    switch( op.type )
    {
    case side:       lights.SetSideColor ( op.value ); break;
    case front:      lights.SetFrontColor( op.value ); break;
    case top:        lights.SetTopColor  ( op.value ); break;
    case brightness: lights.SetBrightness( op.value ); break;
    case pause:      tools::thread::Thread::Sleep( op.value ); break;
    case all:        lights.SetLights( ( op.value & 0xFF000000 ) >> 24,
                                       ( op.value & 0xFF0000 ) >> 16,
                                       ( op.value & 0xFF00 ) >> 8,
                                       ( op.value & 0xFF ) ); break;
    case repeat:     Repeat( ( op.value & 0xFFFF0000 ) >> 16, ( op.value &0xFFFF ), lights ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: Melody::Repeat
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Melody::Repeat( short index, short times, XPSLights& lights ) const
{
    while( times > 0 )
    {
        unsigned i = index;
        while( operations_.at( i ).type != repeat )
        {
            PlayOperation( operations_.at( i ), lights );
            ++i;
        }
        --times;
    }
}
