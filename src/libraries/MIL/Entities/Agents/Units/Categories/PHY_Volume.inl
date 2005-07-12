// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_Volume.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 18:25 $
// $Revision: 2 $
// $Workfile: PHY_Volume.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Volume::FindVolume
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
inline
const PHY_Volume* PHY_Volume::FindVolume( const std::string& strName )
{
    CIT_VolumeMap it = volumes_.find( strName );
    return it == volumes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Volume::FindVolume
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
inline
const PHY_Volume* PHY_Volume::FindVolume( uint nID )
{
    CIT_VolumeMap it = std::find_if( volumes_.begin(), volumes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Volume::GetID ), std::select2nd< T_VolumeMap::value_type >() ) ) );

    return it == volumes_.end() ? 0 : it->second;
}


// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetVolumes
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Volume::T_VolumeMap& PHY_Volume::GetVolumes()
{
    return volumes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Volume::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Volume::GetID
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
inline
uint PHY_Volume::GetID() const
{
    return nID_;
}
