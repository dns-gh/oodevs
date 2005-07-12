//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Object.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_Object.cpp $
//
//*****************************************************************************

#include "MOS_Pch.h"

#include "MOS_Object.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Object.inl"
#endif

#include "MOS_ShapePatch.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Patch.h"

//-----------------------------------------------------------------------------
// Name: MOS_Object constructor
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
MOS_Object::MOS_Object( E_TerrainObjectTypeID nType )
    : nType_     ( nType )
    , nFrame_    ( 0 )
    , rValue_    ( 0. )
    , vPosText_  ()
{
    switch ( nType_ )
    {
        case eTypeTerrainBridge:
        case eTypeTerrainCrossRoad:
        case eTypeTerrainTunnel:
        case eTypeTerrainInterchange:
        {
            nTypeShape_ = eTypeShapeNode;
        }
        break;

        case eTypeTerrainNoway:
        case eTypeTerrainRoadAutoroute:
        case eTypeTerrainRoadNational:
        case eTypeTerrainRoadDepartemental:
        case eTypeTerrainRoadChemin:
        case eTypeTerrainRiverRuisseau:
        case eTypeTerrainRiver:
        case eTypeTerrainRiverBig:
        case eTypeTerrainBridgeBig:
        case eTypeTerrainRailRoad:
        case eTypeTerrainCrest:
        case eTypeTerrainTunnelBig:
        case eTypeTerrainFalaise:
        {
            nTypeShape_ = eTypeShapeSegment;
        }
        break;

        case eTypeTerrainNothing:
        case eTypeTerrainForest:
        case eTypeTerrainForestBig:
        case eTypeTerrainFleuve:
        case eTypeTerrainLake:
        case eTypeTerrainUrban:
        case eTypeTerrainCoast:
        case eTypeTerrainPlantation:
        case eTypeTerrainSwamp:
        case eTypeTerrainGround:
        case eTypeTerrainDune:
        {
            nTypeShape_ = eTypeShapeTri;
        }
        break;

        default :
        {
            assert( false );
        }
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_Object destructor
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
MOS_Object::~MOS_Object()
{
    DeleteAllShapePatchs();
}



//-----------------------------------------------------------------------------
// Name: MOS_Object::DeleteAllShapePatchs
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
void MOS_Object::DeleteAllShapePatchs()
{
    IT_ShapePatchMap itShapePatch = shapePatchMap_.begin();
    while ( itShapePatch != shapePatchMap_.end() )
    {
        MOS_ShapePatch& shapePatch = itShapePatch->second;
        shapePatch.GetPatch().UnregisterShapePatch( shapePatch );
        shapePatchMap_.erase( itShapePatch );
        itShapePatch = shapePatchMap_.begin();
    }

}

//-----------------------------------------------------------------------------
//  Name  :  MOS_Object::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_Object::Read( MT_InputArchive_ABC& archive )
{
    archive.ReadField( "nId_", nId_ );
    archive.ReadField( "sName", sName_ );
//    MT_Float rLengthMeterBridge = 0.;
//    MT_Float rAltitudeCrest = 0.; 
//    MT_Float rWidthMeterRiver = 0.;
    uint nLanesRoad = 0;
    bool bSizeUrban = false;
    switch( nType_ )
    {
        case eTypeTerrainBridge             : archive.ReadField( "rLengthMeter_", rValue_ ); break;
        case eTypeTerrainCrest              : archive.ReadField( "rAltitude_",    rValue_ ); break;
        case eTypeTerrainRiverRuisseau      : archive.ReadField( "rWidthMeter_",  rValue_ ); break;
        case eTypeTerrainRiver              : archive.ReadField( "rWidthMeter_",  rValue_ ); break;
        case eTypeTerrainRiverBig           : archive.ReadField( "rWidthMeter_",  rValue_ ); break;
        case eTypeTerrainRoadAutoroute      : archive.ReadField( "nLanes_",       nLanesRoad );  rValue_ = (MT_Float)nLanesRoad; break;
        case eTypeTerrainRoadDepartemental  : archive.ReadField( "nLanes_",       nLanesRoad );  rValue_ = (MT_Float)nLanesRoad; break;
        case eTypeTerrainRoadNational       : archive.ReadField( "nLanes_",       nLanesRoad );  rValue_ = (MT_Float)nLanesRoad; break;
        case eTypeTerrainRoadChemin         : archive.ReadField( "nLanes_",       nLanesRoad );  rValue_ = (MT_Float)nLanesRoad; break;
        case eTypeTerrainUrban              : archive.ReadField( "bType_",        bSizeUrban );  rValue_ = (MT_Float)bSizeUrban; break;
    }
     

}


//-----------------------------------------------------------------------------
// Name: MOS_Object::ReadGeom
// Created: FBD 03-02-27
//-----------------------------------------------------------------------------
void MOS_Object::ReadGeom( MT_InputArchive_ABC& archive )
{
    MT_Float rXMin = 0.;
    MT_Float rXMax = 0.;
    MT_Float rYMin = 0.;
    MT_Float rYMax = 0.;
    archive.ReadField( "rXMin_", rXMin );
    archive.ReadField( "rXMax_", rXMax );
    archive.ReadField( "rYMin_", rYMin );
    archive.ReadField( "rYMax_", rYMax );
    vPosText_ = MT_Rect( rXMin, rYMin, rXMax, rYMax ).GetCenter();

    MOS_World& world = MOS_App::GetApp().GetWorld();
    // ShapePatchs
    uint nShapePatch = 0;
    archive.ReadField( "nShapePatch", nShapePatch );
    for( uint nIdxShapePatch = 0; nIdxShapePatch < nShapePatch; ++nIdxShapePatch )
    {

        uint nIdxPatchX = 0;
        uint nIdxPatchY = 0;
        archive.ReadField( "nIdxPatchX", nIdxPatchX );
        archive.ReadField( "nIdxPatchY", nIdxPatchY );
        MOS_Patch* pPatch = world.GetPatchFromIdx( nIdxPatchX, nIdxPatchY );
        assert( pPatch );
        MOS_ShapePatch shapePatch( *this, *pPatch );
        shapePatch.Read( archive );
        std::pair< IT_ShapePatchMap, bool > thePair = shapePatchMap_.insert( std::make_pair( pPatch, shapePatch ) );
        if ( thePair.second )
        {
            IT_ShapePatchMap itShape = thePair.first;
            pPatch->RegisterShapePatch( itShape->second );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Object::DrawObjectsName
// Created: FBD 02-12-02
//-----------------------------------------------------------------------------
void MOS_Object::DrawObjectsName( MT_Float rDistPercent )
{
    GFX_Color baseColor = GFX_Tools::GetColorForObjectType( nType_ );
    const std::string& sName = sName_;
    if ( sName != "" )
    {
        MT_Float rSize = 1.0;
        if ( nType_ == eTypeTerrainUrban )
        {
            if ( rValue_ != 0 )
                rSize = 4.0;
            else
                rSize = 3.;
        }

        GFX_Tools::PrintWithShadows( vPosText_, rSize, 4.0, 8., baseColor, false, sName.c_str() );
    }
}


