//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Tools.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 11:03 $
// $Revision: 15 $
// $Workfile: MIL_Tools.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentServer.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "protocol/Protocol.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/FileWrapper.h"
#include "Tools/MIL_Tools.h"
#include <sys/timeb.h>

MIL_Tools::converter< PHY_RawVisionData::E_VisionObject > MIL_Tools::environnementConverter_[] =
{
    converter< PHY_RawVisionData::E_VisionObject >( "Vide",   PHY_RawVisionData::eVisionEmpty ),
    converter< PHY_RawVisionData::E_VisionObject >( "Foret",  PHY_RawVisionData::eVisionForest ),
    converter< PHY_RawVisionData::E_VisionObject >( "Urbain", PHY_RawVisionData::eVisionUrban ),
    converter< PHY_RawVisionData::E_VisionObject >( "Sol",    PHY_RawVisionData::eVisionGround ),
    converter< PHY_RawVisionData::E_VisionObject >( "",      (PHY_RawVisionData::E_VisionObject)-1 )
};

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetAltitude
// Created: JVT 03-03-17
//-----------------------------------------------------------------------------
double MIL_Tools::GetAltitude( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const weather::WindData& MIL_Tools::GetWind( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetWind( vPos );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordMosToSim( const std::string& strMosPos, MT_Vector2D& vSimPos )
{
    assert( strMosPos.size() == 15 );
    TER_World::GetWorld().MosToSimMgrsCoord( strMosPos, vSimPos );
}

//-----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordMosToSim
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordMosToSim( const sword::CoordLatLong& asn, MT_Vector2D& vSimPos )
{
    TER_World::GetWorld().MosToSimMgrsCoord( asn.latitude(), asn.longitude(), vSimPos );
}

//-----------------------------------------------------------------------------
// Name: std::string
// Created: NLD 2002-08-06
// Last modified: JVT 02-08-08
//-----------------------------------------------------------------------------
std::string MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos )
{
    std::string strMosPos;
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, strMosPos );
    return strMosPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ConvertCoordSimToMos
// Created: AGE 2008-04-02
// -----------------------------------------------------------------------------
void MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos, sword::CoordLatLong& asn )
{
    double x, y;
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, x, y );
    asn.set_latitude( x );
    asn.set_longitude( y );
}
