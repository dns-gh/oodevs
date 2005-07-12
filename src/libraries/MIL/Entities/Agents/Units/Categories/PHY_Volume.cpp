// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_Volume.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:44 $
// $Revision: 3 $
// $Workfile: PHY_Volume.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_Volume.h"

PHY_Volume::T_VolumeMap PHY_Volume::volumes_;
uint                    PHY_Volume::nNextID_ = 0;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Volume::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing volumes" );

    // Initialisation des composantes
    archive.BeginList( "Volumes" );

    while ( archive.NextListElement() )
    {
        archive.Section( "Volume" );

        std::string strVolume;
        archive.Read( strVolume );

        const PHY_Volume*& pVolume = volumes_[ strVolume ];
        if( pVolume )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Volume '%s' already registered", strVolume.c_str() ) );
        
        pVolume = new PHY_Volume( strVolume );

        archive.EndSection(); // Volume
    }
    archive.EndList(); // Volumes
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Volume::Terminate()
{
    for( CIT_VolumeMap itVolume = volumes_.begin(); itVolume != volumes_.end(); ++itVolume )
        delete itVolume->second;
    volumes_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Volume constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Volume::PHY_Volume( const std::string& strName )
    : strName_( strName    )
    , nID_    ( nNextID_++ )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Volume::~PHY_Volume()
{
}
