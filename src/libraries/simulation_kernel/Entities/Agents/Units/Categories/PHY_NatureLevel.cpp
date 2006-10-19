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

PHY_NatureLevel::T_NatureLevelMap PHY_NatureLevel::natureLevels_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureLevel::Initialize()
{
    natureLevels_[ "none"  ] = new PHY_NatureLevel( "none" , eNatureLevel_none , EnumNatureLevel::none  );
    natureLevels_[ "o"     ] = new PHY_NatureLevel( "o"    , eNatureLevel_o    , EnumNatureLevel::o     );
    natureLevels_[ "oo"    ] = new PHY_NatureLevel( "oo"   , eNatureLevel_oo   , EnumNatureLevel::oo    );
    natureLevels_[ "ooo"   ] = new PHY_NatureLevel( "ooo"  , eNatureLevel_ooo  , EnumNatureLevel::ooo   );
    natureLevels_[ "i"     ] = new PHY_NatureLevel( "i"    , eNatureLevel_i    , EnumNatureLevel::i     );
    natureLevels_[ "ii"    ] = new PHY_NatureLevel( "ii"   , eNatureLevel_ii   , EnumNatureLevel::ii    );
    natureLevels_[ "iii"   ] = new PHY_NatureLevel( "iii"  , eNatureLevel_iii  , EnumNatureLevel::iii   );
    natureLevels_[ "x"     ] = new PHY_NatureLevel( "x"    , eNatureLevel_x    , EnumNatureLevel::x     );
    natureLevels_[ "xx"    ] = new PHY_NatureLevel( "xx"   , eNatureLevel_xx   , EnumNatureLevel::xx    );
    natureLevels_[ "xxx"   ] = new PHY_NatureLevel( "xxx"  , eNatureLevel_xxx  , EnumNatureLevel::xxx   );
    natureLevels_[ "xxxx"  ] = new PHY_NatureLevel( "xxxx" , eNatureLevel_xxxx , EnumNatureLevel::xxxx  );
    natureLevels_[ "xxxxx" ] = new PHY_NatureLevel( "xxxxx", eNatureLevel_xxxxx, EnumNatureLevel::xxxxx );
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureLevel::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureLevel::PHY_NatureLevel( const std::string& strName, E_Type nType, ASN1T_EnumNatureLevel nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureLevel destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureLevel::~PHY_NatureLevel()
{
}