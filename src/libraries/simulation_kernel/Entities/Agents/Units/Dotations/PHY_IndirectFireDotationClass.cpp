// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 15:08 $
// $Revision: 1 $
// $Workfile: PHY_IndirectFireDotationClass.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_IndirectFireDotationClass.h"

#include "PHY_DotationCategory_IndirectFire.h"
#include "PHY_DotationCategory_IndirectWeatherFire.h"
#include "PHY_DotationCategory_IndirectMineFire.h"
#include <xeumeuleu/xml.h>



const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::explosif_ ( "Explosif" , eExplosif , /*NeedPH*/true , &PHY_DotationCategory_IndirectFire       ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::grenade_  ( "Grenade"  , eGrenade  , /*NeedPH*/true , &PHY_DotationCategory_IndirectFire       ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::aced_     ( "ACED"     , eACED     , /*NeedPH*/true , &PHY_DotationCategory_IndirectFire       ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::fumigene_ ( "Fumigene" , eFumigene , /*NeedPH*/false, &PHY_DotationCategory_IndirectWeatherFire::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::eclairant_( "Eclairant", eEclairant, /*NeedPH*/false, &PHY_DotationCategory_IndirectWeatherFire::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::mine_     ( "Mine"     , eMine     , /*NeedPH*/false, &PHY_DotationCategory_IndirectMineFire   ::Create );

PHY_IndirectFireDotationClass::T_TypeMap PHY_IndirectFireDotationClass::types_;

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Initialize
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_IndirectFireDotationClass::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing indirect ammo effects" );
    types_[ explosif_ .GetID() ] = &explosif_;
    types_[ grenade_  .GetID() ] = &grenade_;
    types_[ aced_     .GetID() ] = &aced_;
    types_[ fumigene_ .GetID() ] = &fumigene_;
    types_[ eclairant_.GetID() ] = &eclairant_;
    types_[ mine_     .GetID() ] = &mine_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Terminate
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_IndirectFireDotationClass::Terminate()
{
    types_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass constructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_IndirectFireDotationClass::PHY_IndirectFireDotationClass( const std::string& strName, E_Type nType, bool bNeedPH, T_TypeInstancier typeInstancier )
    : strName_       ( strName )
    , nType_         ( nType   )
    , typeInstancier_( typeInstancier )
    , bNeedPH_       ( bNeedPH )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass destructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_IndirectFireDotationClass::~PHY_IndirectFireDotationClass()
{
    // NOTHING
}
