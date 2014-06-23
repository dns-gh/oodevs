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
#include "Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Urban/PHY_MaterialCompositionType.h"

//-----------------------------------------------------------------------------
// Name: PHY_DotationCategory::PHY_DotationCategory
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationCategory::PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, xml::xistream& xis )
    : type_                 ( type )
    , pAmmoDotationClass_   ( 0 )
    , pLogisticType_        ( 0 )
    , pNature_              ( 0 )
    , strName_              ( strName )
    , nMosID_               ( 0 )
    , rWeight_              ( 0. )
    , rVolume_              ( 0. )
    , fRange_               ( 0.)
    , bIlluminating_        ( false )
    , bMaintainIllumination_( false )
    , ied_                  ( false )
    , bGuided_              ( false )
    , bMaintainGuidance_    ( false )
    , rGuidanceRange_       ( 0.f )
    , nInterventionType_    ( 0 )
    , rDispersionX_         ( 0 )
    , rDispersionY_         ( 0 )
    , rDetectionRange_      ( 0 )
{
    std::string strNature;
    xis >> xml::attribute( "id", nMosID_ )
        >> xml::attribute( "nature", strNature )
        >> xml::optional >> xml::attribute( "ied", ied_ );
    pNature_ = PHY_DotationNature::Find( strNature );
    if( !pNature_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation nature" );
    InitializePackagingData   ( xis );
    InitializeAttritions      ( xis );
    InitializeUrbanAttritions ( xis );
    InitializeIndirectFireData( xis );
    InitializeLogisticType    ( xis );
    InitializeIllumination    ( xis );
    InitializeGuidance        ( xis );
    if( !attritions_.empty() || !indirectFireEffects_.empty() )
    {
        if( xis.has_attribute( "type" ) )
        {
            pAmmoDotationClass_ = PHY_AmmoDotationClass::Find( xis.attribute< std::string >( "type" ) );
            if( !pAmmoDotationClass_ )
                throw MASA_EXCEPTION( xis.context() + "Invalid ammo dotation class" );
        }
    }
    else
        pAmmoDotationClass_ = 0;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationCategory::~PHY_DotationCategory
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationCategory::~PHY_DotationCategory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializePackagingData
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializePackagingData( xml::xistream& xis )
{
    double rNbrInPackage;
    xis >> xml::attribute( "package-size", rNbrInPackage )
        >> xml::attribute( "package-mass", rWeight_ )
        >> xml::attribute( "package-volume", rVolume_ );

    if( rNbrInPackage <= 0 )
        throw MASA_EXCEPTION( xis.context() + "dotation rNbrInPackage <= 0" );
    if( rWeight_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "dotation rWeight_ <= 0" );
    if( rVolume_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "dotation rVolume_ <= 0" );

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
    xis >> xml::attribute( "protection", protectionType );

    const PHY_Protection::T_ProtectionMap& protections = PHY_Protection::GetProtections();
    PHY_Protection::CIT_ProtectionMap it = protections.find( protectionType );
    if( it == protections.end() )
        throw MASA_EXCEPTION( xis.context() + "Undefined protection type: " + protectionType );

    const PHY_Protection& protection = *it->second;
    assert( attritions_.size() > protection.GetID() );
    attritions_[ protection.GetID() ] = PHY_AttritionData( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeUrbanAttritions
// Created: SLG 2010-04-14
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeUrbanAttritions( xml::xistream& xis )
{
    struct UrbanModifier
    {
        static void Read( xml::xistream& xis, std::unique_ptr< PHY_UrbanAttritionData >& data )
        {
            data.reset( new PHY_UrbanAttritionData( xis ) );
            data->UpdateGlobalScore();
        }
    };
    xis >> xml::list( "urban-modifiers", boost::bind( &UrbanModifier::Read, _1, boost::ref( urbanAttritionData_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeIndirectFireData( xml::xistream& xis )
{
    if( xis.has_child( "indirect-fires" ) )
    {
        xis >> xml::start( "indirect-fires" )
            >> xml::attribute( "intervention-type", nInterventionType_ )
            >> xml::attribute( "x-dispersion", rDispersionX_ )
            >> xml::attribute( "y-dispersion", rDispersionY_ )
            >> xml::optional >> xml::attribute( "detection-range", rDetectionRange_ );
        if( nInterventionType_ <= 0. )
            throw MASA_EXCEPTION( xis.context() + "intervention-type <= 0" );
        if( rDispersionX_ <= 0. )
            throw MASA_EXCEPTION( xis.context() + "rDispersionX_ <= 0" );
        if( rDispersionY_ <= 0. )
            throw MASA_EXCEPTION( xis.context() + "rDispersionY_ <= 0" );
        if( rDetectionRange_ < 0. )
            throw MASA_EXCEPTION( xis.context() + "rDetectionRange_ < 0" );
        xis >> xml::list( "indirect-fire", *this, &PHY_DotationCategory::ReadIndirectFire, nInterventionType_, rDispersionX_, rDispersionY_, rDetectionRange_ )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ReadIndirectFire
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ReadIndirectFire( xml::xistream& xis, unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange )
{
    const PHY_IndirectFireDotationClass* pType = PHY_IndirectFireDotationClass::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown indirect fire data type" );
    indirectFireEffects_.push_back(  &pType->InstanciateDotationCategory( *this, xis, nInterventionType, rDispersionX, rDispersionY, rDetectionRange ) ); // $$$$ MCO 2012-06-28: never deleted
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeLogisticType
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeLogisticType( xml::xistream& xis )
{
    std::string strLogisticSupplyClass;
    xis >> xml::attribute( "logistic-supply-class", strLogisticSupplyClass );
    pLogisticType_ = PHY_DotationLogisticType::Find( strLogisticSupplyClass );
    if( !pLogisticType_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown logistic supply class" );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeIllumination
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeIllumination( xml::xistream& xis )
{
    if( xis.has_child( "illuminating" ) )
    {
        bIlluminating_ = true;
        xis >> xml::start( "illuminating" )
                >> xml::attribute( "range", fRange_ )
                >> xml::attribute( "maintain", bMaintainIllumination_ )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeGuidance
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeGuidance( xml::xistream& xis )
{
    if( xis.has_child( "guided" ) )
    {
        bGuided_ = true;
        xis >> xml::start( "guided" )
                >> xml::attribute( "maintain", bMaintainGuidance_ )
                >> xml::attribute( "range", rGuidanceRange_ )
            >> xml::end;
    }
}

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
// Name: PHY_DotationCategory::PHY_DotationCategory::GetUrbanAttritionScore
// Created: JCR 2011-08-12
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetUrbanAttritionScore( const PHY_MaterialCompositionType& material ) const
{
    return urbanAttritionData_.get() ? urbanAttritionData_->GetScore( material ) : 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetAttritionScore
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetAttritionScore( const PHY_Protection& protectionTarget ) const
{
    return GetAttritionData( protectionTarget ).GetScore();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetIndirectFireEffects
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory::T_IndirectFireEffects& PHY_DotationCategory::GetIndirectFireEffects() const
{
    return indirectFireEffects_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ApplyIndirectFireEffect
// Created: ABR 2011-01-19
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ApplyIndirectFireEffect( const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const
{
    const double rInterventionTypeFired = ConvertToInterventionType( nNbrAmmoFired );
    for( auto it = indirectFireEffects_.begin(); it != indirectFireEffects_.end(); ++ it )
        (*it)->ApplyEffect( 0, vSourcePosition, vTargetPosition, rInterventionTypeFired, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ApplyIndirectFireEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ApplyIndirectFireEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const
{
    const double rInterventionTypeFired = ConvertToInterventionType( nNbrAmmoFired );
    for( auto it = indirectFireEffects_.begin(); it != indirectFireEffects_.end(); ++ it )
        (*it)->ApplyEffect( &firer, vSourcePosition, vTargetPosition, rInterventionTypeFired, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ApplyStrikeEffect
// Created: MGD 2010-10-15
// -----------------------------------------------------------------------------
void PHY_DotationCategory::ApplyStrikeEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const
{
    const double rInterventionTypeFired = ConvertToInterventionType( nNbrAmmoFired );
    for( auto it = indirectFireEffects_.begin(); it != indirectFireEffects_.end(); ++ it )
        (*it)->ApplyStrikeEffect( firer, target, rInterventionTypeFired, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetNature
// Created: NLD 2006-03-21
// -----------------------------------------------------------------------------
const PHY_DotationNature& PHY_DotationCategory::GetNature() const
{
    assert( pNature_ );
    return *pNature_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetLogisticType
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType& PHY_DotationCategory::GetLogisticType() const
{
    assert( pLogisticType_ );
    return *pLogisticType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetAmmoDotationClass
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
const PHY_AmmoDotationClass* PHY_DotationCategory::GetAmmoDotationClass() const
{
    return pAmmoDotationClass_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetMosID
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
unsigned int PHY_DotationCategory::GetMosID() const
{
    return nMosID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetType
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
const PHY_DotationType& PHY_DotationCategory::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetName
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
const std::string& PHY_DotationCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::CanBeUsedForDirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::CanBeUsedForDirectFire() const
{
    return !attritions_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::HasAttritions
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::HasAttritions() const
{
    return !attritions_.empty();
}

// -----------------------------------------------------------------------------

// Name: PHY_DotationCategory::GetSmokeDuration
// Created: LDC 2013-08-30
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetSmokeDuration() const
{
    double result = 0.;
    for( auto it = indirectFireEffects_.begin(); it != indirectFireEffects_.end(); ++ it )
        result = std::max( result, (*it)->GetSmokeDuration() );
    return result;
}

// Name: PHY_DotationCategory::HasIndirectWeaponCategory
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::HasIndirectWeaponCategory( const PHY_IndirectFireDotationClass& indirectWeaponCategory ) const
{
    for( auto it = indirectFireEffects_.begin(); it != indirectFireEffects_.end(); ++ it )
        if( (*it)->GetIndirectFireDotationCategory() == indirectWeaponCategory )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::CanBeUsedForIndirectFire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::CanBeUsedForIndirectFire() const
{
    return !indirectFireEffects_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::operator==( const PHY_DotationCategory& rhs ) const
{
    return this == &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::operator!=( const PHY_DotationCategory& rhs ) const
{
    return this != &rhs;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetWeight
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetWeight() const
{
    return rWeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetVolume
// Created: NLD 2005-07-13
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetVolume() const
{
    return rVolume_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::IsGuided
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::IsGuided() const
{
    return bGuided_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::IsIED
// Created: LDC 2012-06-06
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::IsIED() const
{
    return ied_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetGuidanceRange
// Created: MGD 2010-02-27
// -----------------------------------------------------------------------------
float PHY_DotationCategory::GetGuidanceRange() const
{
  return rGuidanceRange_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::IsIlluminating
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::IsIlluminating( float range, bool permanent ) const
{
    return bIlluminating_ && range < fRange_ && permanent != bMaintainIllumination_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetilluminatingRange
// Created: GGE 2010-06-23
// -----------------------------------------------------------------------------
float PHY_DotationCategory::GetIlluminatingRange( ) const
{
    return fRange_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::FindUrbanProtection
// Created: DDA 2010-04-26
// -----------------------------------------------------------------------------
double PHY_DotationCategory::FindUrbanAttritionScore( const PHY_MaterialCompositionType& material )
{
    return PHY_UrbanAttritionData::GetGlobalScore( material );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::IsSignificantChange
// Created: LDC 2012-02-08
// -----------------------------------------------------------------------------
bool PHY_DotationCategory::IsSignificantChange( double newValue, double oldValue, double capacity ) const
{
    if( &type_ != PHY_DotationType::ration_ && &type_ != PHY_DotationType::carburant_ || capacity == 0 || newValue == capacity )
        return static_cast< unsigned int >( oldValue ) != static_cast< unsigned int >( newValue );
    if( newValue == 0 )
        return true;
    return std::abs( oldValue - newValue ) >= capacity * 0.05; // $$$$ MCO 2013-06-07: hard-coded 5% threshold
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ConvertToNbrAmmo
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
double PHY_DotationCategory::ConvertToNbrAmmo( double rNbrIT ) const
{
    return nInterventionType_ * rNbrIT;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::ConvertToInterventionType
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
double PHY_DotationCategory::ConvertToInterventionType( unsigned int nNbr ) const
{
    return static_cast< float >( nNbr ) / static_cast< float >( nInterventionType_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::GetRadius
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
double PHY_DotationCategory::GetRadius() const
{
    return ( rDispersionX_ + rDispersionY_ ) / 2;
}
