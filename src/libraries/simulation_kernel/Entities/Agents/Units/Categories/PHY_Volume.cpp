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

#include "simulation_kernel_pch.h"

#include "PHY_Volume.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_Volume::T_VolumeMap PHY_Volume::volumes_;
uint                    PHY_Volume::nNextID_ = 0;

struct PHY_Volume::LoadingWrapper
{
    void ReadVolume( xml::xistream& xis )
    {
        PHY_Volume::ReadVolume( xis );
    }
};

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Volume::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing volumes" );

    LoadingWrapper loader;

    // Initialisation des composantes
    xis >> start( "volumes" )
            >> xml::list( "volume", loader, &LoadingWrapper::ReadVolume )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::ReadVolume
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Volume::ReadVolume( xml::xistream& xis )
{
    std::string strVolume;
    xis >> attribute( "name", strVolume );

    const PHY_Volume*& pVolume = volumes_[ strVolume ];
    if( pVolume )
        xis.error( "Volume '" + strVolume + "' already registered" );
    
    pVolume = new PHY_Volume( strVolume );
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
