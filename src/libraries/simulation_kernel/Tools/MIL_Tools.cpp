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

#include "Tools/MIL_Tools.h"

#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "MIL_AgentServer.h"
#include "simulation_terrain/TER_World.h"
#include <sys/timeb.h>

MIL_Tools::converter< PHY_RawVisionData::E_VisionObject > MIL_Tools::environnementConverter_[] =
{
    converter< PHY_RawVisionData::E_VisionObject >( "Vide",   PHY_RawVisionData::eVisionEmpty       ),
    converter< PHY_RawVisionData::E_VisionObject >( "Foret",  PHY_RawVisionData::eVisionForest      ),
    converter< PHY_RawVisionData::E_VisionObject >( "Urbain", PHY_RawVisionData::eVisionUrban       ),
    converter< PHY_RawVisionData::E_VisionObject >( "Sol"   , PHY_RawVisionData::eVisionGround      ),
    converter< PHY_RawVisionData::E_VisionObject >( "",       (PHY_RawVisionData::E_VisionObject)-1 )
};

//-----------------------------------------------------------------------------
// Name: MIL_Tools::GetAltitude
// Created: JVT 03-03-17
//-----------------------------------------------------------------------------
MT_Float MIL_Tools::GetAltitude( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::GetWind
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
const PHY_Meteo::sWindData& MIL_Tools::GetWind( const MT_Vector2D& vPos )
{
    return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetWind( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Tools::ComputeCRC
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
boost::crc_32_type::value_type MIL_Tools::ComputeCRC( const std::string& fileName )
{
    static const uint nBufferSize = 4096;

    char               buffer[ nBufferSize ];
    boost::crc_32_type CRC;

    std::ifstream file( fileName.c_str(), std::ios::in | std::ios::binary );

    while ( file )
    {
        file.read( buffer, nBufferSize );
        CRC.process_bytes( buffer, file.gcount() );
    }

    file.close();

    return CRC.checksum();
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
void MIL_Tools::ConvertCoordMosToSim( const ASN1T_CoordLatLong& asn, MT_Vector2D& vSimPos )
{
    TER_World::GetWorld().MosToSimMgrsCoord( asn.latitude, asn.longitude, vSimPos );
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
void MIL_Tools::ConvertCoordSimToMos( const MT_Vector2D& vSimPos, ASN1T_CoordLatLong& asn )
{
    TER_World::GetWorld().SimToMosMgrsCoord( vSimPos, asn.latitude, asn.longitude );

}

