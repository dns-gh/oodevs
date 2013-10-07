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
#include "PHY_DotationCategory_IndirectObjectCreationFire.h"
#include "PHY_DotationCategory_IndirectWeatherFire.h"
#include "MT_Tools/MT_Logger.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::explosif_  ( "Explosif"  , eExplosif  , /*NeedPH*/true , &PHY_DotationCategory_IndirectFire              ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::fumigene_  ( "Fumigene"  , eFumigene  , /*NeedPH*/false, &PHY_DotationCategory_IndirectWeatherFire       ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::eclairant_ ( "Eclairant" , eEclairant , /*NeedPH*/false, &PHY_DotationCategory_IndirectWeatherFire       ::Create );
const PHY_IndirectFireDotationClass PHY_IndirectFireDotationClass::effect_    ( "Effect"    , eEffect    , /*NeedPH*/false, &PHY_DotationCategory_IndirectObjectCreationFire::Create );

PHY_IndirectFireDotationClass::T_TypeMap PHY_IndirectFireDotationClass::types_;

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Initialize
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_IndirectFireDotationClass::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing indirect ammo effects" );
    types_[ explosif_  .GetID() ] = &explosif_;
    types_[ fumigene_  .GetID() ] = &fumigene_;
    types_[ eclairant_ .GetID() ] = &eclairant_;
    types_[ effect_    .GetID() ] = &effect_;
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

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Find
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_IndirectFireDotationClass* PHY_IndirectFireDotationClass::Find( int nID )
{
    auto it = types_.find( nID );
    if( it == types_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::Find
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
const PHY_IndirectFireDotationClass* PHY_IndirectFireDotationClass::Find( const std::string& strName )
{
    for( auto it = types_.begin(); it != types_.end(); ++it )
    {
        if( boost::iequals( strName, it->second->GetName() ) )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::InstanciateWeaponDataType
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_DotationCategory_IndirectFire_ABC& PHY_IndirectFireDotationClass::InstanciateDotationCategory( const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                                                                   unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange) const
{
    return typeInstancier_( *this, dotationCategory, xis, nInterventionType, rDispersionX, rDispersionY, rDetectionRange );
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::GetID
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
int PHY_IndirectFireDotationClass::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::GetName
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
const std::string& PHY_IndirectFireDotationClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_IndirectFireDotationClass::operator==( const PHY_IndirectFireDotationClass& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_IndirectFireDotationClass::operator!=( const PHY_IndirectFireDotationClass& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireDotationClass::NeedPH
// Created: NLD 2005-08-03
// -----------------------------------------------------------------------------
bool PHY_IndirectFireDotationClass::NeedPH() const
{
    return bNeedPH_;
}
