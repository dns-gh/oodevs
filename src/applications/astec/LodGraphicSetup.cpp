// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/LodGraphicSetup.cpp $
// $Author: Age $
// $Modtime: 24/06/05 16:59 $
// $Revision: 2 $
// $Workfile: LodGraphicSetup.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "LodGraphicSetup.h"
#include "graphics/GraphicData.h"

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup constructor
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
LodGraphicSetup::LodGraphicSetup()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup destructor
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
LodGraphicSetup::~LodGraphicSetup()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup::SetupLineGraphics
// Created: AGE 2005-06-24
// -----------------------------------------------------------------------------
void LodGraphicSetup::SetupLineGraphics( const Data_ABC* pData )
{
    const GraphicData* pRealData = static_cast< const GraphicData* >( pData );
    if( pRealData )
        nLastLod_ = GetLodFor( *pRealData );
    GraphicSetup::SetupLineGraphics( pData );
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup::SetupBorderGraphics
// Created: AGE 2005-06-24
// -----------------------------------------------------------------------------
void LodGraphicSetup::SetupBorderGraphics( const Data_ABC* pData )
{
    GraphicSetup::SetupBorderGraphics( pData );
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup::SetupAreaGraphics
// Created: AGE 2005-06-24
// -----------------------------------------------------------------------------
void LodGraphicSetup::SetupAreaGraphics( const Data_ABC* pData )
{
    const GraphicData* pRealData = static_cast< const GraphicData* >( pData );
    if( pRealData )
        nLastLod_ = GetLodFor( *pRealData );
    GraphicSetup::SetupAreaGraphics( pData );
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup::GetLodFor
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
unsigned int LodGraphicSetup::GetLodFor( const TerrainData& data )
{
    if( data == TerrainData::Forest() )
        return 0;
    if( data == TerrainData::Plantation() )
        return 0;
    if( data == TerrainData::Swamp() )
        return 0;
    if( data == TerrainData::Dune() )
        return 0;
    if( data == TerrainData::Urban() )
        return 0;
    if( data == TerrainData::Water() )
        return 0;
    if( data == TerrainData::Ice() )
        return 0;
    if( data == TerrainData::Cliff() )
        return 1;
    if( data == TerrainData::Motorway() )
        return 1;
    if( data == TerrainData::LargeRoad() )
        return 1;
    if( data == TerrainData::MediumRoad() )
        return 2;
    if( data == TerrainData::SmallRoad() )
        return 2;
    if( data == TerrainData::Bridge() )
        return 2;
    if( data == TerrainData::Railroad() )
        return 1;
    if( data == TerrainData::LargeRiver() )
        return 1;
    if( data == TerrainData::MediumRiver() )
        return 1;
    if( data == TerrainData::SmallRiver() )
        return 2;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LodGraphicSetup::GetLastLevelOfDetail
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
unsigned int LodGraphicSetup::GetLastLevelOfDetail() const
{
    return nLastLod_;
}
