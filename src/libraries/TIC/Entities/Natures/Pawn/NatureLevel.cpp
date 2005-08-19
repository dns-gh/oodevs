// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureLevel.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureLevel.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "NatureLevel.h"

using namespace TIC;

NatureLevel::T_NatureLevelMap NatureLevel::natureLevel_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureLevel::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureLevel::Initialize()
{
    natureLevel_[ "none"  ] = new NatureLevel( "none" , eNatureLevel_none , EnumNatureLevel::none , "NKN"   );
    natureLevel_[ "o"     ] = new NatureLevel( "o"    , eNatureLevel_o    , EnumNatureLevel::o    , "TEAM"  );
    natureLevel_[ "oo"    ] = new NatureLevel( "oo"   , eNatureLevel_oo   , EnumNatureLevel::oo   , "SECT"  );
    natureLevel_[ "ooo"   ] = new NatureLevel( "ooo"  , eNatureLevel_ooo  , EnumNatureLevel::ooo  , "PLT"   );
    natureLevel_[ "i"     ] = new NatureLevel( "i"    , eNatureLevel_i    , EnumNatureLevel::i    , "COY"   );
    natureLevel_[ "ii"    ] = new NatureLevel( "ii"   , eNatureLevel_ii   , EnumNatureLevel::ii   , "BN"    );
    natureLevel_[ "iii"   ] = new NatureLevel( "iii"  , eNatureLevel_iii  , EnumNatureLevel::iii  , "RGT"   );
    natureLevel_[ "x"     ] = new NatureLevel( "x"    , eNatureLevel_x    , EnumNatureLevel::x    , "BDE"   );
    natureLevel_[ "xx"    ] = new NatureLevel( "xx"   , eNatureLevel_xx   , EnumNatureLevel::xx   , "DIV"   );
    natureLevel_[ "xxx"   ] = new NatureLevel( "xxx"  , eNatureLevel_xxx  , EnumNatureLevel::xxx  , "CORPS" );
    natureLevel_[ "xxxx"  ] = new NatureLevel( "xxxx" , eNatureLevel_xxxx , EnumNatureLevel::xxxx , "ARMY"  );
    natureLevel_[ "xxxxx" ] = new NatureLevel( "xxxxx", eNatureLevel_xxxxx, EnumNatureLevel::xxxxx, "AG"    );
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureLevel::Terminate()
{
    for( CIT_NatureLevelMap it = natureLevel_.begin(); it != natureLevel_.end(); ++it )
        delete it->second;
    natureLevel_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureLevel constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureLevel::NatureLevel( const std::string& strName, E_Type nType, ASN1T_EnumNatureLevel nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureLevel destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureLevel::~NatureLevel()
{
}