// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 15:24 $
// $Revision: 6 $
// $Workfile: PHY_DotationCategory.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_DotationCategory.h"

#include "PHY_DotationType.h"
#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "PHY_IndirectFireDotationClass.h"
#include "PHY_AmmoDotationClass.h"
#include "PHY_DotationLogisticType.h"
#include "PHY_DotationNature.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "xeumeuleu/xml.h"

using namespace xml;


//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationCategory::PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, xml::xistream& xis )
    : type_              ( type )
    , pAmmoDotationClass_( 0 )
    , pLogisticType_     ( 0 )
    , pNature_           ( 0 )
    , strName_           ( strName )
    , nMosID_            ( 0 )
    , pIndirectFireData_ ( 0 )
    , attritions_        ()
    , rWeight_           ( 0. )
    , rVolume_           ( 0. ) 
{
    std::string strNature;
    xis >> attribute( "id", nMosID_ )
        >> attribute( "nature", strNature );

    pNature_ = PHY_DotationNature::Find( strNature );
    if( !pNature_ )
        xis.error( "Unknown dotation nature" );

    InitializePackagingData   ( xis );
    InitializeAttritions      ( xis );
    InitializeIndirectFireData( xis );
    InitializeLogisticType    ( xis );

    if( !attritions_.empty() || pIndirectFireData_ )
    {
        std::string strTmp;
        xis >> attribute( "type", strTmp );
        pAmmoDotationClass_ = PHY_AmmoDotationClass::Find( strTmp );
        if( !pAmmoDotationClass_ )
            xis.error( "Invalid ammo dotation class" );
    }
    else
        pAmmoDotationClass_ = 0;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationCategory::~PHY_DotationCategory()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializePackagingData
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializePackagingData( xml::xistream& xis )
{
    MT_Float rNbrInPackage;
    xis >> attribute( "package-size", rNbrInPackage )
        >> attribute( "package-mass", rWeight_ )
        >> attribute( "package-volume", rVolume_ );

    if( rNbrInPackage <= 0 )
        xis.error( "dotation rNbrInPackage <= 0" );
    if( rWeight_ <= 0 )
        xis.error( "dotation rWeight_ <= 0" );
    if( rVolume_ <= 0 )
        xis.error( "dotation rVolume_ <= 0" );

    rWeight_ /= rNbrInPackage;
    rVolume_ /= rNbrInPackage;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeAttritions
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeAttritions( xml::xistream& xis )
{
    attritions_.clear();
    xis >> xml::list( "attritions", *this, &PHY_DotationCategory::ListAttrition );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ListAttrition
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ListAttrition( xml::xistream& xis )
{
    xis >> xml::list( "attrition", *this, &PHY_DotationCategory::ReadAttrition );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ReadAttrition
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ReadAttrition( xml::xistream& xis )
{
    attritions_.resize( PHY_Protection::GetProtections().size() );
    std::string protectionType;
    xis >> attribute( "protection", protectionType );

    const PHY_Protection::T_ProtectionMap& protections = PHY_Protection::GetProtections();
    PHY_Protection::CIT_ProtectionMap it = protections.find( protectionType );
    if( it == protections.end() )
        xis.error( "Undefined protection type: " + protectionType );

    const PHY_Protection& protection = *it->second;
    assert( attritions_.size() > protection.GetID() );
    attritions_[ protection.GetID() ] = PHY_AttritionData( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeIndirectFireData( xml::xistream& xis )
{
    xis >> xml::list( "indirect-fire", *this, &PHY_DotationCategory::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ReadIndirectFire
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ReadIndirectFire( xml::xistream& xis )
{
    pIndirectFireData_ = 0;
    std::string strType;

    xis >> attribute( "type", strType );

    const PHY_IndirectFireDotationClass* pType = PHY_IndirectFireDotationClass::Find( strType );
    if( !pType )
        xis.error( "Unknown indirect fire data type" );

    pIndirectFireData_ = &pType->InstanciateDotationCategory( *this, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeLogisticType
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeLogisticType( xml::xistream& xis )
{
    pLogisticType_ = &type_.GetDefaultLogisticType();
    bool dTranche = false;

    xis >> optional() >> attribute( "d-type", dTranche );
    if( dTranche )
        pLogisticType_ = &PHY_DotationLogisticType::uniteFeuTD_;
}

// =============================================================================
// FIRE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetAttritionData
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
const PHY_AttritionData& PHY_DotationCategory::GetAttritionData( const PHY_Protection& protectionTarget ) const
{
    assert( attritions_.size() > protectionTarget.GetID() );
    return attritions_[ protectionTarget.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetAttritionScore
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
MT_Float PHY_DotationCategory::GetAttritionScore( const PHY_Protection& protectionTarget ) const
{
    return GetAttritionData( protectionTarget ).GetScore();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ApplyIndirectFireEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ApplyIndirectFireEffect( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const
{
    assert( pIndirectFireData_ );
    const MT_Float rInterventionTypeFired = pIndirectFireData_->ConvertToInterventionType( nNbrAmmoFired );
    pIndirectFireData_->ApplyEffect( firer, vSourcePosition, vTargetPosition, rInterventionTypeFired, fireResult );
}

