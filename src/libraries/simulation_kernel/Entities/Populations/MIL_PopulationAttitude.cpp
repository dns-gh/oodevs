// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Humans/MIL_PopulationAttitude.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 14:01 $
// $Revision: 2 $
// $Workfile: MIL_PopulationAttitude.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationAttitude.h"

MIL_PopulationAttitude::T_AttitudeMap MIL_PopulationAttitude::attitudes_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Initialize
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void MIL_PopulationAttitude::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing population attitudes" );
    attitudes_[ "calme"     ] = new MIL_PopulationAttitude( "calme"    , eCalme    , Common::EnumPopulationAttitude::calme     );
    attitudes_[ "agitee"    ] = new MIL_PopulationAttitude( "agitee"   , eAgitee   , Common::EnumPopulationAttitude::agitee    );
    attitudes_[ "excitee"   ] = new MIL_PopulationAttitude( "excitee"  , eExcitee  , Common::EnumPopulationAttitude::excitee   );
    attitudes_[ "agressive" ] = new MIL_PopulationAttitude( "agressive", eAgressive, Common::EnumPopulationAttitude::agressive );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Terminate
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
void MIL_PopulationAttitude::Terminate()
{
    for( CIT_AttitudeMap it = attitudes_.begin(); it != attitudes_.end(); ++it )
        delete it->second;
    attitudes_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationAttitude::MIL_PopulationAttitude( const std::string& strName, E_Attitude nAttitude, Common::EnumPopulationAttitude nAsnID )
    : strName_  ( strName   )
    , nAttitude_( nAttitude )
    , nAsnID_   ( nAsnID    )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude destructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
MIL_PopulationAttitude::~MIL_PopulationAttitude()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetAttitudes
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude::T_AttitudeMap& MIL_PopulationAttitude::GetAttitudes()
{
    return attitudes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( const std::string& strName )
{
    CIT_AttitudeMap it = attitudes_.find( strName );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( Common::EnumPopulationAttitude nAsnID )
{
    CIT_AttitudeMap it = std::find_if( attitudes_.begin(), attitudes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nAsnID ), std::compose1( std::mem_fun( &MIL_PopulationAttitude::GetAsnID ), std::select2nd< T_AttitudeMap::value_type >() ) ) );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::Find
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude* MIL_PopulationAttitude::Find( unsigned int nID )
{
    CIT_AttitudeMap it = std::find_if( attitudes_.begin(), attitudes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &MIL_PopulationAttitude::GetID ), std::select2nd< T_AttitudeMap::value_type >() ) ) );
    return it == attitudes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetName
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
const std::string& MIL_PopulationAttitude::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetAsnID
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
Common::EnumPopulationAttitude MIL_PopulationAttitude::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::GetID
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
unsigned int MIL_PopulationAttitude::GetID() const
{
    return (unsigned int)nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator==
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator==( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ == rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator!=
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator!=( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ != rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator>
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator>( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ > rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator>=
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator>=( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ >= rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator<
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator<( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ < rhs.nAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationAttitude::operator<=
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
bool MIL_PopulationAttitude::operator<=( const MIL_PopulationAttitude& rhs ) const
{
    return nAttitude_ <= rhs.nAttitude_;
}
