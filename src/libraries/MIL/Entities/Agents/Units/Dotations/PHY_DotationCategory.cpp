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

#include "MIL_Pch.h"

#include "PHY_DotationCategory.h"
#include "PHY_DotationCategory_IndirectFire_ABC.h"
#include "PHY_IndirectFireDotationClass.h"
#include "PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationCategory::PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, MIL_InputArchive& archive )
    : type_              ( type )
    , pAmmoDotationClass_( 0 )
    , strName_           ( strName )
    , nMosID_            ( 0 )
    , pIndirectFireData_ ( 0 )
    , attritions_        ()
    , rWeight_           ( 0. )
    , rVolume_           ( 0. ) 
{
    archive.ReadField( "MosID"                   , nMosID_   );

    InitializePackagingData   ( archive );
    InitializeAttritions      ( archive );
    InitializeIndirectFireData( archive );

    if( !attritions_.empty() || pIndirectFireData_ )
    {
        std::string strTmp;
        archive.ReadField( "Type", strTmp );
        pAmmoDotationClass_ = PHY_AmmoDotationClass::Find( strTmp );
        if( !pAmmoDotationClass_ )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Invalid ammo dotation class", archive.GetContext() );
    }
    else
        pAmmoDotationClass_ = &PHY_AmmoDotationClass::none_;
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
void PHY_DotationCategory::InitializePackagingData( MIL_InputArchive& archive )
{
    MT_Float rNbrInPackage;
    archive.Section( "Conditionnement" );
    archive.ReadField( "Nombre", rNbrInPackage, CheckValueGreater( 0. ) );
    archive.ReadField( "Masse" , rWeight_, CheckValueGreater( 0. ) );
    archive.ReadField( "Volume", rVolume_, CheckValueGreater( 0. ) );
    archive.EndSection(); // Conditionnement

    rWeight_ /= rNbrInPackage;
    rVolume_ /= rNbrInPackage;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeAttritions
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeAttritions( MIL_InputArchive& archive )
{
    attritions_.clear();

    if( !archive.Section( "Attritions", MIL_InputArchive::eNothing ) )
        return;

    attritions_.resize( PHY_Protection::GetProtections().size() );

    const PHY_Protection::T_ProtectionMap& protections = PHY_Protection::GetProtections();
    for( PHY_Protection::CIT_ProtectionMap itProtection = protections.begin(); itProtection != protections.end(); ++itProtection )
    {
        const PHY_Protection& protection = *itProtection->second;

        std::stringstream strSectionName;
        strSectionName << "ProtectionType" << protection.GetName();

        archive.Section( strSectionName.str() );

        assert( attritions_.size() > protection.GetID() );
        attritions_[ protection.GetID() ] = PHY_AttritionData( archive );

        archive.EndSection(); // ProtectionTypeXXX
    }
    archive.EndSection(); // Attritions
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::InitializeIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_DotationCategory::InitializeIndirectFireData( MIL_InputArchive& archive )
{
    pIndirectFireData_ = 0;

    if( !archive.Section( "TirIndirect", MIL_InputArchive::eNothing ) )
        return;

    std::string strType;
    archive.ReadAttribute( "type", strType );

    const PHY_IndirectFireDotationClass* pType = PHY_IndirectFireDotationClass::Find( strType );
    if( !pType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown indirect fire data type", archive.GetContext() );

    pIndirectFireData_ = &pType->InstanciateDotationCategory( *this, archive );      
    archive.EndSection(); // TirIndirect
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
// Name: PHY_DotationCategory::IndirectFire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_DotationCategory::IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const
{
    assert( pIndirectFireData_ );
    const MT_Float rInterventionTypeFired = pIndirectFireData_->ConvertToInterventionType( nNbrAmmoFired );
    pIndirectFireData_->Fire( firer, vSourcePosition, vTargetPosition, rInterventionTypeFired, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationCategory::IndirectFire
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_DotationCategory::IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmoFired ) const
{
    assert( pIndirectFireData_ );
    const MT_Float rInterventionTypeFired = pIndirectFireData_->ConvertToInterventionType( nNbrAmmoFired );
    pIndirectFireData_->Fire( firer, vSourcePosition, vTargetPosition, rInterventionTypeFired );
}
