// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_CoordinateManager.cpp $
// $Author: Age $
// $Modtime: 25/05/05 13:30 $
// $Revision: 4 $
// $Workfile: TER_CoordinateManager.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_CoordinateManager.h"
#include "MT/MT_Logger/MT_LogDefines.h"
#include "MT/MT_Logger/MT_LogManager.h"

#include <cmath>

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager constructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_CoordinateManager::TER_CoordinateManager( float rMiddleLatitude, float rMiddleLongitude, const MT_Rect& extent )
    : parameters_( rMiddleLatitude * std::acos( -1. ) / 180., rMiddleLongitude * std::acos( -1. ) / 180. ) 
    , planar_( parameters_ )
    , extent_( extent )
    , translation_( extent.GetWidth() * 0.5, extent.GetHeight() * 0.5 )
{
    SimToMosMgrsCoord( MT_Vector2D( 0, 0 ), strDefaultMGRS_ );
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager destructor
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
TER_CoordinateManager::~TER_CoordinateManager()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::MosToSimMgrsCoord
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_CoordinateManager::MosToSimMgrsCoord( const std::string& strMgrs, MT_Vector2D& pos ) const
{
    try
    {
        mgrs_.SetString( strMgrs );
        planar_.SetCoordinates( mgrs_ );
        pos.rX_ = planar_.GetX();
        pos.rY_ = planar_.GetY();
        pos += translation_;
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in " __FUNCTION__ " converting " << strMgrs << " to sim position : " << e.what() );
        pos = MT_Vector2D( 0, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::SimToMosMgrsCoord
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_CoordinateManager::SimToMosMgrsCoord( const MT_Vector2D& pos, std::string& strMgrs ) const
{
    try
    {
        const MT_Vector2D translated = pos - translation_;
        planar_.Set( translated.rX_, translated.rY_ );
        mgrs_.SetCoordinates( planar_ );
        strMgrs = mgrs_.GetString();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in " __FUNCTION__ " converting (" << pos.rX_ << ", " << pos.rY_ << ") to mgrs : " << e.what() );
        strMgrs = strDefaultMGRS_;
    }
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::IsValidPosition
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
bool TER_CoordinateManager::IsValidPosition( const MT_Vector2D& pos ) const
{
    return extent_.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_CoordinateManager::ClipPointInsideWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
void TER_CoordinateManager::ClipPointInsideWorld( MT_Vector2D& pos ) const
{
    extent_.ClipPoint( pos );
}
