// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_VirtualObjectType_ControlZone.cpp $
// $Author: Nld $
// $Modtime: 18/11/04 11:21 $
// $Revision: 2 $
// $Workfile: MIL_VirtualObjectType_ControlZone.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_VirtualObjectType_ControlZone.h"
#include "MIL_ControlZone.h"

#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType_ControlZone constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_VirtualObjectType_ControlZone::MIL_VirtualObjectType_ControlZone( const std::string& strName )
    : MIL_VirtualObjectType( strName )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType_ControlZone destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_VirtualObjectType_ControlZone::~MIL_VirtualObjectType_ControlZone()
{

}


// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType_ControlZone::Read
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_VirtualObjectType_ControlZone::Read( MIL_InputArchive& archive )
{
    vUnitDensityFirePercentage_.clear();
    vUnitDensityFirePercentage_.resize( PHY_Volume::GetVolumes().size(), 0. );

    archive.BeginList( "PourcentagesTirParHumainParHectare" );
    while ( archive.NextListElement() )
    {
        archive.Section( "Pourcentage" );

        std::string strVolume;
        archive.ReadAttribute( "volume", strVolume );
        const PHY_Volume* pVolume = PHY_Volume::FindVolume( strVolume );
        if ( !pVolume )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown volume name", archive.GetContext() );

        MT_Float rUnitDensityFirePercentage;
        archive.Read( rUnitDensityFirePercentage, CheckValueBound( 0., 100. ) );
        
        rUnitDensityFirePercentage *= 10000.;                                                          // hectare => m2
        rUnitDensityFirePercentage = MIL_Tools::ConvertMeterSquareToSim( rUnitDensityFirePercentage ); // m2 => px2
       
        vUnitDensityFirePercentage_[ pVolume->GetID() ] = rUnitDensityFirePercentage / 100.;

        archive.EndSection(); // Pourcentage
    }
    archive.EndList(); // PourcentagesTirParHumainParHectare

    bInitialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObjectType_ControlZone::InstanciateObject
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MT_Float MIL_VirtualObjectType_ControlZone::GetUnitDensityFirePercentage( const PHY_Volume& volume ) const
{
    assert( volume.GetID() < vUnitDensityFirePercentage_.size() );
    return vUnitDensityFirePercentage_[ volume.GetID() ];
}


