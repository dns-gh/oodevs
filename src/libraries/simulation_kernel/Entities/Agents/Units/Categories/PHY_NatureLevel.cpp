// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureLevel.cpp $
// $Author: Nld $
// $Modtime: 6/04/05 10:45 $
// $Revision: 3 $
// $Workfile: PHY_NatureLevel.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_NatureLevel.h"
#include "protocol/protocol.h"

PHY_NatureLevel::T_NatureLevelMap PHY_NatureLevel::natureLevels_;

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureLevel::Initialize()
{
    natureLevels_[ "none"  ] = new PHY_NatureLevel( "none" , eNatureLevel_none , Common::none_naturelevel  );
    natureLevels_[ "o"     ] = new PHY_NatureLevel( "o"    , eNatureLevel_o    , Common::o     );
    natureLevels_[ "oo"    ] = new PHY_NatureLevel( "oo"   , eNatureLevel_oo   , Common::oo    );
    natureLevels_[ "ooo"   ] = new PHY_NatureLevel( "ooo"  , eNatureLevel_ooo  , Common::ooo   );
    natureLevels_[ "i"     ] = new PHY_NatureLevel( "i"    , eNatureLevel_i    , Common::i     );
    natureLevels_[ "ii"    ] = new PHY_NatureLevel( "ii"   , eNatureLevel_ii   , Common::ii    );
    natureLevels_[ "iii"   ] = new PHY_NatureLevel( "iii"  , eNatureLevel_iii  , Common::iii   );
    natureLevels_[ "x"     ] = new PHY_NatureLevel( "x"    , eNatureLevel_x    , Common::x     );
    natureLevels_[ "xx"    ] = new PHY_NatureLevel( "xx"   , eNatureLevel_xx   , Common::xx    );
    natureLevels_[ "xxx"   ] = new PHY_NatureLevel( "xxx"  , eNatureLevel_xxx  , Common::xxx   );
    natureLevels_[ "xxxx"  ] = new PHY_NatureLevel( "xxxx" , eNatureLevel_xxxx , Common::xxxx  );
    natureLevels_[ "xxxxx" ] = new PHY_NatureLevel( "xxxxx", eNatureLevel_xxxxx, Common::xxxxx );
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureLevel::Terminate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureLevel::PHY_NatureLevel( const std::string& strName, E_Type nType, Common::EnumNatureLevel nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureLevel::~PHY_NatureLevel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_NatureLevel* PHY_NatureLevel::Find( const std::string& strName )
{
    CIT_NatureLevelMap it = natureLevels_.find( strName );
    return it == natureLevels_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: phy_naturelevel::Find
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
const PHY_NatureLevel* PHY_NatureLevel::Find( unsigned int nID )
{
    for( CIT_NatureLevelMap it = natureLevels_.begin(); it != natureLevels_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const std::string& PHY_NatureLevel::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
unsigned int PHY_NatureLevel::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
Common::EnumNatureLevel PHY_NatureLevel::GetAsnID() const
{
    return nAsnID_;
}
