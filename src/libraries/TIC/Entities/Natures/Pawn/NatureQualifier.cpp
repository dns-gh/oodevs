// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureQualifier.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureQualifier.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "NatureQualifier.h"

using namespace TIC;

NatureQualifier::T_NatureQualifierMap NatureQualifier::natureQualifier_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureQualifier::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureQualifier::Initialize()
{
    natureQualifier_[ "Lourd"  ] = new NatureQualifier( "Lourd"  , eLourd, EnumUnitNatureQualifier::lourd, "HEAVY"  );
    natureQualifier_[ "Leger"  ] = new NatureQualifier( "Leger"  , eLeger, EnumUnitNatureQualifier::leger, "LIGHT"  );
    natureQualifier_[ "Moyen"  ] = new NatureQualifier( "Moyen"  , eMoyen, EnumUnitNatureQualifier::moyen, "MEDIUM" );
    natureQualifier_[ "None"   ] = new NatureQualifier( "None"   , eNone,  EnumUnitNatureQualifier::none , "NONE"   );
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void NatureQualifier::Terminate()
{
    for( CIT_NatureQualifierMap it = natureQualifier_.begin(); it != natureQualifier_.end(); ++it )
        delete it->second;
    natureQualifier_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NatureQualifier constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureQualifier::NatureQualifier( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureQualifier nAsnID, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
NatureQualifier::~NatureQualifier()
{
}