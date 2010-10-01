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
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

PHY_Volume::T_VolumeMap PHY_Volume::volumes_;
unsigned int                    PHY_Volume::nNextID_ = 0;

struct PHY_Volume::LoadingWrapper
{
    void ReadVolume( xml::xistream& xis )
    {
        PHY_Volume::ReadVolume( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Volume::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing volumes" );
    LoadingWrapper loader;
    xis >> xml::start( "volumes" )
            >> xml::list( "volume", loader, &LoadingWrapper::ReadVolume )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::ReadVolume
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Volume::ReadVolume( xml::xistream& xis )
{
    std::string strVolume;
    xis >> xml::attribute( "name", strVolume );
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

// -----------------------------------------------------------------------------
// Name: PHY_Volume constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Volume::PHY_Volume( const std::string& strName )
    : strName_( strName    )
    , nID_    ( nNextID_++ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_Volume::~PHY_Volume()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::FindVolume
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_Volume* PHY_Volume::FindVolume( const std::string& strName )
{
    CIT_VolumeMap it = volumes_.find( strName );
    return it == volumes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::FindVolume
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Volume* PHY_Volume::FindVolume( unsigned int nID )
{
    CIT_VolumeMap it = std::find_if( volumes_.begin(), volumes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_Volume::GetID ), std::select2nd< T_VolumeMap::value_type >() ) ) );

    return it == volumes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetVolumes
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_Volume::T_VolumeMap& PHY_Volume::GetVolumes()
{
    return volumes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Volume::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetID
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
unsigned int PHY_Volume::GetID() const
{
    return nID_;
}
