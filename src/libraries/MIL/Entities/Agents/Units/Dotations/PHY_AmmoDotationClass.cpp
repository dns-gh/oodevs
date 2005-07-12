// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 16:40 $
// $Revision: 2 $
// $Workfile: PHY_AmmoDotationClass.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_AmmoDotationClass.h"

#include "PHY_DotationCategory_IndirectFire.h"
#include "PHY_DotationCategory_IndirectWeatherFire.h"
#include "PHY_DotationCategory_IndirectMineFire.h"


const PHY_AmmoDotationClass PHY_AmmoDotationClass::obus_      ( "Obus"      , eObus       );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::missileAir_( "MissileAir", eMissileAir );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::missileSol_( "MissileSol", eMissileSol );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::mitraille_ ( "Mitraille" , eMitraille  );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::none_      ( "None"      , eNone       );

PHY_AmmoDotationClass::T_TypeMap PHY_AmmoDotationClass::types_;

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Initialize
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_AmmoDotationClass::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing ammo dotation classes" );

    types_[ obus_      .GetID() ] = &obus_;
    types_[ missileAir_.GetID() ] = &missileAir_;
    types_[ missileSol_.GetID() ] = &missileSol_;
    types_[ mitraille_ .GetID() ] = &mitraille_;
    types_[ none_      .GetID() ] = &none_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Terminate
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_AmmoDotationClass::Terminate()
{
    types_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass constructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_AmmoDotationClass::PHY_AmmoDotationClass( const std::string& strName, E_Type nType )
    : strName_( strName )
    , nType_  ( nType   )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass destructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_AmmoDotationClass::~PHY_AmmoDotationClass()
{
}
