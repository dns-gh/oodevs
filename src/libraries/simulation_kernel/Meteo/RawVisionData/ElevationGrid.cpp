// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ElevationGrid.h"
#include "PHY_AmmoEffect.h"
#include "meteo/Meteo.h"

const weather::Meteo* ElevationCell::pGlobalMeteo_ = 0;
ElevationCell ElevationGrid::emptyCell_;

// -----------------------------------------------------------------------------
// Name: ElevationGrid constructor
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::ElevationGrid( double cellSize, unsigned int width, unsigned int height, ElevationCell** ppCells )
    : ElevationBaseGrid( cellSize, width, height )
    , ppCells_( ppCells )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid destructor
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationGrid::~ElevationGrid()
{
    for( unsigned int i = width_; i; )
        delete [] ppCells_[ --i ];
    delete [] ppCells_;
    ppCells_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetCellAltitude
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
short ElevationGrid::GetCellAltitude( unsigned int col, unsigned int row ) const
{
    return GetCell( col, row ).GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetCell
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationCell& ElevationGrid::GetCell( unsigned int x, unsigned int y ) const
{
    return ( x < width_ && y < height_ ) ? ppCells_[ x ][ y ] : emptyCell_;
}

// -----------------------------------------------------------------------------
// Name: ElevationGrid::GetEmptyCell
// Created: LGY 2013-02-04
// -----------------------------------------------------------------------------
ElevationCell& ElevationGrid::GetEmptyCell() const
{
    return emptyCell_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::sCell::GetPrecipitation
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Precipitation& ElevationCell::GetPrecipitation() const
{
    const weather::PHY_Precipitation& mainPrecipitation = pMeteo ? pMeteo->GetPrecipitation() : pGlobalMeteo_->GetPrecipitation();
    return pEffects ? pEffects->GetPrecipitation( mainPrecipitation ) : mainPrecipitation;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetLighting
// Created: JVT 04-03-24
//-----------------------------------------------------------------------------
const weather::PHY_Lighting& ElevationCell::GetLighting() const
{
    const weather::PHY_Lighting& mainLighting = pMeteo ? pMeteo->GetLighting() : pGlobalMeteo_->GetLighting();
    return pEffects ? pEffects->GetLighting( mainLighting ) : mainLighting;
}

// -----------------------------------------------------------------------------
// Name: PHY_RawVisionData::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& ElevationCell::GetWind() const
{
    return pMeteo ? pMeteo->GetWind() : pGlobalMeteo_->GetWind();
}

