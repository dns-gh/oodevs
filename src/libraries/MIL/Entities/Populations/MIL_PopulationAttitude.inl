// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_PopulationAttitude.inl $
// $Author: Jvt $
// $Modtime: 14/04/05 17:06 $
// $Revision: 3 $
// $Workfile: MIL_PopulationAttitude.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetAttitudes
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude::T_AttitudeMap& MIL_PopulationAttitude::GetAttitudes()
{
    return attitudes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( const std::string& strName )
{
    CIT_AttitudeMap it = attitudes_.find( strName );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( ASN1T_EnumPopulationAttitude nAsnID )
{
    CIT_AttitudeMap it = std::find_if( attitudes_.begin(), attitudes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nAsnID ), std::compose1( std::mem_fun( &MIL_PopulationAttitude::GetAsnID ), std::select2nd< T_AttitudeMap::value_type >() ) ) );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
inline
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( uint nID )
{
    CIT_AttitudeMap it = std::find_if( attitudes_.begin(), attitudes_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &MIL_PopulationAttitude::GetID ), std::select2nd< T_AttitudeMap::value_type >() ) ) );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
const std::string& MIL_PopulationAttitude::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetAsnID
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
ASN1T_EnumPopulationAttitude MIL_PopulationAttitude::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
inline
uint MIL_PopulationAttitude::GetID() const
{
    return (uint)nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator==
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationAttitude::operator==( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ == rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator!=
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool MIL_PopulationAttitude::operator!=( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ != rhs.nAttitude_;
}
