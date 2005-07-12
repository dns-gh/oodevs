// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_NatureQualifier.cpp $
// $Author: Nld $
// $Modtime: 4/04/05 13:07 $
// $Revision: 3 $
// $Workfile: PHY_NatureQualifier.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_NatureQualifier.h"

PHY_NatureQualifier::T_NatureQualifierMap PHY_NatureQualifier::natureQualifier_;

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureQualifier::Initialize()
{
    natureQualifier_[ "Lourd"  ] = new PHY_NatureQualifier( "Lourd"  , eLourd, EnumUnitNatureQualifier::lourd );
    natureQualifier_[ "Leger"  ] = new PHY_NatureQualifier( "Leger"  , eLeger, EnumUnitNatureQualifier::leger );
    natureQualifier_[ "Moyen"  ] = new PHY_NatureQualifier( "Moyen"  , eMoyen, EnumUnitNatureQualifier::moyen );
    natureQualifier_[ "None"   ] = new PHY_NatureQualifier( "None"   , eNone,  EnumUnitNatureQualifier::none  );
}


// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_NatureQualifier::Terminate()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureQualifier::PHY_NatureQualifier( const std::string& strName, E_Type nType, ASN1T_EnumUnitNatureQualifier nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_NatureQualifier destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_NatureQualifier::~PHY_NatureQualifier()
{
}