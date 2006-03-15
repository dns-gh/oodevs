// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ElevationLayer.h"
#include "graphics/DrawDetection.h"

// -----------------------------------------------------------------------------
// Name: ElevationLayer constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ElevationLayer::ElevationLayer( const std::string& detectionFile )
    : detectionFile_( detectionFile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationLayer destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ElevationLayer::~ElevationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationLayer::Initialize
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ElevationLayer::Initialize( const geometry::Rectangle2f& extent )
{
    detection_.reset( new DrawDetection( detectionFile_, extent ) );
}

// -----------------------------------------------------------------------------
// Name: ElevationLayer::Paint
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ElevationLayer::Paint( const geometry::Rectangle2f& )
{
    if( detection_.get() )
        detection_->Draw();
}
