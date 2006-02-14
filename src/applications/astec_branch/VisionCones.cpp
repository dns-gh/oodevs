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

// -----------------------------------------------------------------------------
// Name: VisionCones constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::VisionCones()
    : needsUpdating_( false )
{

}
    
// -----------------------------------------------------------------------------
// Name: VisionCones destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
VisionCones::~VisionCones()
{

}

// -----------------------------------------------------------------------------
// Name: VisionCones::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::Update( const VisionConesMessage& message )
{
    for( CIT_Surfaces itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    surfaces_.clear();

    unsigned long nNbrSurfaces;
    message >> nNbrSurfaces;
    surfaces_.reserve( nNbrSurfaces );
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_.push_back( new Surface( message ) );

    message >> elongationFactor_;
    needsUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: VisionCones::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void VisionCones::Update( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.positionPresent 
     || message.m.experiencePresent 
     || message.m.fatiguePresent )
        needsUpdating_ = true;
}
