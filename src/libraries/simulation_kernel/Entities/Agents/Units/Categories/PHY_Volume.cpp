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

PHY_Volume::T_VolumeMap PHY_Volume::volumes_;
unsigned int                    PHY_Volume::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: PHY_Volume::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_Volume::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing volumes" );
    xis >> xml::start( "volumes" )
            >> xml::list( "volume", &PHY_Volume::ReadVolume )
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
        throw MASA_EXCEPTION( xis.context() + "Volume '" + strVolume + "' already registered" );
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
    for( auto it = volumes_.begin(); it != volumes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
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
