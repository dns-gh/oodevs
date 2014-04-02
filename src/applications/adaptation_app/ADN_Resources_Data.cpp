// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_Data.h"
#include "ADN_Tools.h"
#include "ADN_Categories_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"
#include "protocol/Simulation.h"
#include "ENT/ENT_Tr.h"

ADN_Resources_Data::ResourceInfos* gpDummyDotationInfos;
tools::IdManager ADN_Resources_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::CategoryInfo
/*
*   This horrible constructor is necessary due to the ADN_Wizard_FirstPage_Default
*   but is never used.
*/
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo::CategoryInfo()
    : nId_( 0 )
    , parentResource_( *gpDummyDotationInfos )
    , ptrResourceNature_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Natures_Data >( eNatures ).GetNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses(), 0 )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eResources, "resources" ) );
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CategoryInfo
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo::CategoryInfo( ResourceInfos& parentDotation )
    : nId_                   ( ADN_Resources_Data::idManager_.GetNextId() )
    , parentResource_        ( parentDotation )
    , category_              ( ENT_Tr::ConvertFromDotationCategory( parentDotation.nType_ ) )
    , strCodeEMAT6_          ()
    , strCodeEMAT8_          ()
    , strCodeLFRIL_          ()
    , strCodeNNO_            ()
    , ptrResourceNature_     ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Natures_Data >( eNatures ).GetNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses(), 0 )
    , rNbrInPackage_         ( 1. )
    , rPackageVolume_        ( 1. )
    , rPackageWeight_        ( 1. )
    , bNetworkUsable_        ( false )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eResources, "resources" ) );
    BindExistenceTo( &ptrResourceNature_ );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CategoryInfo
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo::CategoryInfo( ResourceInfos& parentDotation, unsigned int id )
    : nId_                   ( id )
    , parentResource_        ( parentDotation )
    , category_              ( ENT_Tr::ConvertFromDotationCategory( parentDotation.nType_ ) )
    , strCodeEMAT6_          ()
    , strCodeEMAT8_          ()
    , strCodeLFRIL_          ()
    , strCodeNNO_            ()
    , ptrResourceNature_     ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Natures_Data >( eNatures ).GetNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses(), 0 )
    , rNbrInPackage_         ( 1. )
    , rPackageVolume_        ( 1. )
    , rPackageWeight_        ( 1. )
    , bNetworkUsable_        ( false )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eResources, "resources" ) );
    BindExistenceTo( &ptrResourceNature_ );
    ADN_Resources_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::~CategoryInfo
// Created: JSR 2013-07-29
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo::~CategoryInfo()
{
    parentResource_.NotifyDestroyed( this );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CreateCopy
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo* ADN_Resources_Data::CategoryInfo::CreateCopy()
{
    CategoryInfo* pCopy = new CategoryInfo( parentResource_ );
    pCopy->rNbrInPackage_  = rNbrInPackage_ .GetData();
    pCopy->rPackageVolume_ = rPackageVolume_.GetData();
    pCopy->rPackageWeight_ = rPackageWeight_.GetData();
    pCopy->ptrResourceNature_ = ptrResourceNature_.GetData();
    pCopy->ptrLogisticSupplyClass_ = ptrLogisticSupplyClass_.GetData();
    pCopy->strCodeEMAT6_ = strCodeEMAT6_.GetData();
    pCopy->strCodeEMAT8_ = strCodeEMAT8_.GetData();
    pCopy->strCodeLFRIL_ = strCodeLFRIL_.GetData();
    pCopy->strCodeNNO_ = strCodeNNO_.GetData();
    pCopy->bNetworkUsable_ = bNetworkUsable_.GetData();
    if( bNetworkUsable_.GetData() )
        parentResource_.AddUsableInNetwork( pCopy );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Resources_Data::CategoryInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "category", category_ );
    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_   = strName_.GetData();
    std::string dotationNature, logisticSupplyClass;
    input >> xml::optional >> xml::attribute( "codeEMAT6", strCodeEMAT6_ )
          >> xml::optional >> xml::attribute( "codeEMAT8", strCodeEMAT8_ )
          >> xml::optional >> xml::attribute( "codeLFRIL", strCodeLFRIL_ )
          >> xml::optional >> xml::attribute( "codeNNO", strCodeNNO_ )
          >> xml::attribute( "package-size", rNbrInPackage_ )
          >> xml::attribute( "package-mass", rPackageWeight_ )
          >> xml::attribute( "package-volume", rPackageVolume_ )
          >> xml::attribute( "nature", ptrResourceNature_ )
          >> xml::attribute( "logistic-supply-class", ptrLogisticSupplyClass_ )
          >> xml::attribute( "network-usable", bNetworkUsable_ );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Resources_Data::CategoryInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" );
    WriteContent( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Resources_Data::CategoryInfo::Initialize()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data:::CategoryInfo::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Resources_Data::CategoryInfo::WriteContent( xml::xostream& output ) const
{
    output << xml::attribute( "category", category_ )
           << xml::attribute( "name", *this )
           << xml::attribute( "id", nId_ )
           << xml::attribute( "package-size", rNbrInPackage_ )
           << xml::attribute( "package-mass", rPackageWeight_ )
           << xml::attribute( "package-volume", rPackageVolume_ )
           << xml::attribute( "nature", ptrResourceNature_ )
           << xml::attribute( "logistic-supply-class", ptrLogisticSupplyClass_ )
           << xml::attribute( "id-nature", ptrResourceNature_.GetData() ? ptrResourceNature_.GetData()->nId_.GetData() : 0 )
           << xml::attribute( "codeEMAT6", strCodeEMAT6_ )
           << xml::attribute( "codeEMAT8", strCodeEMAT8_ )
           << xml::attribute( "codeLFRIL", strCodeLFRIL_ )
           << xml::attribute( "codeNNO", strCodeNNO_ )
           << xml::attribute( "network-usable", bNetworkUsable_ );
}

//-----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ModificatorPostureInfos
// Created: JDY 03-09-29
//-----------------------------------------------------------------------------
ADN_Resources_Data::ModificatorPostureInfos::ModificatorPostureInfos( const E_UnitPosture& e )
    : eType_ ( e )
    , rCoeff_(1.0)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ModificatorPostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ModificatorPostureInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "ph" )
            << xml::attribute( "target-posture", ADN_Tools::ComputePostureScriptName( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::IndirectAmmoInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::IndirectAmmoInfos::IndirectAmmoInfos()
    : bExplosive_          ( false )
    , bSmoke_              ( false )
    , bFlare_              ( false )
    , bEffect_             ( false )
    , nIntervention_       ( 1 )
    , rDispersionX_        ( 1.0 )
    , rDispersionY_        ( 1.0 )
    , rDetectionRange_     ( 5000 )
    , rNeutralizationRatio_( 1 )
    , rExplosiveDispersion_( 0 )
    , rFlareDispersion_    ( 0 )
    , rSmokeDispersion_    ( 0 )
    , flareDeployTime_     ( "0s" )
    , flareLifeTime_       ( "0s" )
    , smokeDeployTime_     ( "0s" )
    , smokeLifeTime_       ( "0s" )
    , nMineNumber_         ( 0 )
    , effectLifeTime_      ( "0s" )
    , objectType_          ( ADN_TypePtr_InVector_ABC< ADN_Objects_Data_ObjectInfos >( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0 ) )
{
    for( int i = 0; i < eNbrUnitPosture; ++i )
    {
        ModificatorPostureInfos* pNew = new ModificatorPostureInfos( ( E_UnitPosture )i );
        std::auto_ptr< ModificatorPostureInfos > spNew( pNew );
        vModifStance_.AddItem( spNew.release() );
    }
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::~IndirectAmmoInfos
// Created: JSR 2013-07-29
// -----------------------------------------------------------------------------
ADN_Resources_Data::IndirectAmmoInfos::~IndirectAmmoInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::CopyFrom
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::CopyFrom( ADN_Resources_Data::IndirectAmmoInfos& ammoInfos )
{
    bExplosive_ = ammoInfos.bExplosive_.GetData();
    bSmoke_ = ammoInfos.bSmoke_.GetData();
    bFlare_ = ammoInfos.bFlare_.GetData();
    bEffect_ = ammoInfos.bEffect_.GetData();

    nIntervention_ = ammoInfos.nIntervention_.GetData();
    rDispersionX_ = ammoInfos.rDispersionX_.GetData();
    rDispersionY_ = ammoInfos.rDispersionY_.GetData();
    rDetectionRange_ = ammoInfos.rDetectionRange_.GetData();
    rNeutralizationRatio_ = ammoInfos.rNeutralizationRatio_.GetData();
    rExplosiveDispersion_ = ammoInfos.rExplosiveDispersion_.GetData();
    rFlareDispersion_ = ammoInfos.rFlareDispersion_.GetData();
    rSmokeDispersion_ = ammoInfos.rSmokeDispersion_.GetData();

    for( uint i=0 ; i< eNbrUnitPosture ; ++i)
        vModifStance_[i]->rCoeff_ = ammoInfos.vModifStance_[ i ]->rCoeff_.GetData();

    flareDeployTime_ = ammoInfos.flareDeployTime_.GetData();
    flareLifeTime_ = ammoInfos.flareLifeTime_.GetData();
    smokeDeployTime_ = ammoInfos.smokeDeployTime_.GetData();
    smokeLifeTime_ = ammoInfos.smokeLifeTime_.GetData();
    nMineNumber_ = ammoInfos.nMineNumber_.GetData();
    objectType_ = ammoInfos.objectType_.GetData();
    strObjectType_ = ammoInfos.strObjectType_;
    effectLifeTime_ = ammoInfos.effectLifeTime_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::IndirectAmmoInfos::ReadPh
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::ReadPh( xml::xistream& input )
{
    std::string posture = input.attribute< std::string >( "target-posture" );
    for( int i = 0; i < eNbrUnitPosture; ++i )
        if( ADN_Tools::ComputePostureScriptName( E_UnitPosture( i ) ) == posture )
        {
            vModifStance_[i]->ReadArchive( input );
            return;
        }
    throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid stance '%1'" ).arg( posture.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "intervention-type", nIntervention_ )
          >> xml::attribute( "x-dispersion", rDispersionX_ )
          >> xml::attribute( "y-dispersion", rDispersionY_ )
          >> xml::optional >> xml::attribute( "detection-range", rDetectionRange_ )
          >> xml::list( "indirect-fire", *this, &ADN_Resources_Data::IndirectAmmoInfos::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadIndirectFire
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::ReadIndirectFire( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    E_TypeMunitionTirIndirect indirect = ADN_Tr::ConvertToTypeMunitionTirIndirect( type );
    if( indirect == (E_TypeMunitionTirIndirect)-1 )
        throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid indirect fire ammo type '%1'" ).arg( type.c_str() ).toStdString() );

    switch( indirect )
    {
        case eTypeMunitionTirIndirect_Explosif:
            bExplosive_ = true;
            input >> xml::attribute( "neutralization-ratio", rNeutralizationRatio_ )
                  >> xml::optional >> xml::attribute( "dispersion-factor", rExplosiveDispersion_ )
                  >> xml::list( "ph", *this, &ADN_Resources_Data::IndirectAmmoInfos::ReadPh );
            break;
        case eTypeMunitionTirIndirect_Fumigene:
            bSmoke_ = true;
            input >> xml::attribute( "setup-time", smokeDeployTime_ )
                  >> xml::optional >> xml::attribute( "dispersion-factor", rSmokeDispersion_ )
                  >> xml::attribute( "life-time", smokeLifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Eclairant:
            bFlare_ = true;
            input >> xml::attribute( "setup-time", flareDeployTime_ )
                  >> xml::optional >> xml::attribute( "dispersion-factor", rFlareDispersion_ )
                  >> xml::attribute( "life-time", flareLifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Effect:
            bEffect_ = true;
            input >> xml::attribute( "object-type", strObjectType_ )
                  >> xml::attribute( "life-time", effectLifeTime_ );
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "indirect-fires" )
                << xml::attribute( "intervention-type", nIntervention_ )
                << xml::attribute( "x-dispersion", rDispersionX_ )
                << xml::attribute( "y-dispersion", rDispersionY_ )
                << xml::attribute( "detection-range", rDetectionRange_ );

    if( bExplosive_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Explosif ) )
                    << xml::attribute( "neutralization-ratio", rNeutralizationRatio_ )
                    << xml::attribute( "dispersion-factor", rExplosiveDispersion_ );
        for( auto it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
            ( *it )->WriteArchive( output );
        output << xml::end;
    }
    if( bSmoke_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Fumigene ) )
                    << xml::attribute( "setup-time", smokeDeployTime_ )
                    << xml::attribute( "life-time", smokeLifeTime_ );
        if( rSmokeDispersion_ != 0 )
            output << xml::attribute( "dispersion-factor", rSmokeDispersion_ );
        output << xml::end;
    }
    if( bFlare_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Eclairant ) )
                    << xml::attribute( "setup-time", flareDeployTime_ )
                    << xml::attribute( "life-time", flareLifeTime_ );
        if( rFlareDispersion_ != 0 )
            output << xml::attribute( "dispersion-factor", rFlareDispersion_ );
        output << xml::end;
    }
    if( bEffect_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Effect ) )
                    << xml::attribute( "object-type", ( objectType_.GetData() != 0 ) ? objectType_.GetData()->strType_.GetData() : "" )
                    << xml::attribute( "life-time", effectLifeTime_ )
                << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Resources_Data::IndirectAmmoInfos::Initialize()
{
    if( !strObjectType_.empty() )
    {
        ADN_Objects_Data_ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( strObjectType_ );
        if( pObject )
        {
            if( pObject != objectType_.GetData() )
                objectType_ = pObject;
        }
        else
            QMessageBox::warning( QApplication::activeWindow(), tools::translate( "Resources_Data", "Invalid data" ), tools::translate( "Resources_Data", "Object type - Invalid object '%1'" ).arg( strObjectType_.c_str() ), QMessageBox::Ok, Qt::NoButton );
    }
    else if( !ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos().empty() )
    {
        objectType_ = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos().front();
        strObjectType_ = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos().front()->strType_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::AmmoCategoryInfo
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::AmmoCategoryInfo::AmmoCategoryInfo( ResourceInfos& parentDotation )
    : CategoryInfo( parentDotation )
    , bIED_                 ( false )
    , bDirect_              ( false )
    , bIndirect_            ( false )
    , bIlluminating_        ( false )
    , fRange_               ( 0 )
    , bMaintainIllumination_( false )
    , bGuided_              ( false )
    , bMaintainGuidance_    ( false )
    , fGuidanceRange_       ( 0 )
    , attritions_           ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorInfosWithoutCrowd() )
    , modifUrbanBlocks_     ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    BindExistenceTo( &( indirectAmmoInfos_.objectType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::AmmoCategoryInfo
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::AmmoCategoryInfo::AmmoCategoryInfo( ResourceInfos& parentDotation, unsigned int id )
    : CategoryInfo( parentDotation, id )
    , bIED_                 ( false )
    , bDirect_              ( false )
    , bIndirect_            ( false )
    , bIlluminating_        ( false )
    , fRange_               ( 0 )
    , bMaintainIllumination_( false )
    , bGuided_              ( false )
    , bMaintainGuidance_    ( false )
    , fGuidanceRange_       ( 0 )
    , attritions_           ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorInfosWithoutCrowd() )
    , modifUrbanBlocks_     ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    BindExistenceTo( &( indirectAmmoInfos_.objectType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::~AmmoCategoryInfo
// Created: JSR 2013-07-29
// -----------------------------------------------------------------------------
ADN_Resources_Data::AmmoCategoryInfo::~AmmoCategoryInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::CreateCopy
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo* ADN_Resources_Data::AmmoCategoryInfo::CreateCopy()
{
    AmmoCategoryInfo* pCopy = new AmmoCategoryInfo( parentResource_ );
    pCopy->nType_ = nType_.GetData();
    pCopy->bIED_ = bIED_.GetData();
    pCopy->bDirect_ = bDirect_.GetData();
    pCopy->bIndirect_ = bIndirect_.GetData();

    pCopy->ptrResourceNature_ = ptrResourceNature_.GetData();
    pCopy->ptrLogisticSupplyClass_ = ptrLogisticSupplyClass_.GetData();

    pCopy->rNbrInPackage_  = rNbrInPackage_ .GetData();
    pCopy->rPackageVolume_ = rPackageVolume_.GetData();
    pCopy->rPackageWeight_ = rPackageWeight_.GetData();

    pCopy->strCodeEMAT6_  = strCodeEMAT6_.GetData();
    pCopy->strCodeEMAT8_  = strCodeEMAT8_.GetData();
    pCopy->strCodeLFRIL_  = strCodeLFRIL_.GetData();
    pCopy->strCodeNNO_    = strCodeNNO_.GetData();
    pCopy->bNetworkUsable_ = bNetworkUsable_.GetData();
    if( bNetworkUsable_.GetData() )
        parentResource_.AddUsableInNetwork( pCopy );

    pCopy->bIlluminating_ = bIlluminating_.GetData();
    pCopy->bMaintainIllumination_ = bMaintainIllumination_.GetData();
    pCopy->fRange_ = fRange_.GetData();

    pCopy->bGuided_ = bGuided_.GetData();
    pCopy->bMaintainGuidance_ = bMaintainGuidance_.GetData();
    pCopy->fGuidanceRange_ = fGuidanceRange_.GetData();

    for( unsigned int i = 0; i < attritions_.size(); ++i )
        pCopy->attritions_[ i ]->CopyFrom( * attritions_[ i ] );

    for( unsigned int i = 0; i < modifUrbanBlocks_.size(); ++i )
        pCopy->modifUrbanBlocks_[ i ]->CopyFrom( *modifUrbanBlocks_[ i ] );

    pCopy->indirectAmmoInfos_.CopyFrom( indirectAmmoInfos_ );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::AmmoCategoryInfo::ReadAttrition
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Resources_Data::AmmoCategoryInfo::ReadAttrition( xml::xistream& input )
{
    bDirect_ = true;
    std::string protection = input.attribute< std::string >( "protection" );
    auto itAttrition = std::find_if( attritions_.begin(), attritions_.end(), helpers::AttritionInfos::Cmp( protection ));
    if( itAttrition == attritions_.end() )
        throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).toStdString() );
    (*itAttrition)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::AmmoCategoryInfo::ReadUrbanModifer
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
void ADN_Resources_Data::AmmoCategoryInfo::ReadUrbanModifer( xml::xistream& input )
{
    std::string material = input.attribute< std::string >( "material-type" );
    auto it = std::find_if( modifUrbanBlocks_.begin(), modifUrbanBlocks_.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
    if( it == modifUrbanBlocks_.end() )
        throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid urban Material type '%1'" ).arg( material.c_str() ).toStdString() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::AmmoCategoryInfo::ReadArchive( xml::xistream& input )
{
    CategoryInfo::ReadArchive( input );

    std::string type;
    input >> xml::optional >> xml::attribute( "type", type );
    input >> xml::optional >> xml::attribute( "ied", bIED_ );
    if( !type.empty() )
    {
        nType_ = ENT_Tr::ConvertToAmmunitionType( type );
        if( nType_ == E_AmmunitionType( -1 ) )
            throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid resource type '%1'" ).arg( type.c_str() ).toStdString() );
    }

    if( input.has_child( "illuminating" ) )
    {
        bIlluminating_ = true;
        input >> xml::start( "illuminating" )
                >> xml::attribute( "range", fRange_ )
                >> xml::attribute( "maintain", bMaintainIllumination_ )
              >> xml::end;
    }
    if( input.has_child( "guided" ) )
    {
        bGuided_ = true;
        input >> xml::start( "guided" )
                >> xml::attribute( "maintain", bMaintainGuidance_ )
                >> xml::attribute( "range", fGuidanceRange_ )
              >> xml::end;
    }

    input >> xml::optional
          >> xml::start( "attritions" )
            >> xml::list( "attrition", *this, &ADN_Resources_Data::AmmoCategoryInfo::ReadAttrition )
          >> xml::end
          >> xml::optional
          >> xml::start( "urban-modifiers" )
            >> xml::list( "urban-modifier", *this, &ADN_Resources_Data::AmmoCategoryInfo::ReadUrbanModifer )
          >> xml::end;
    if( input.has_child( "indirect-fires" ) )
    {
        bIndirect_ = true;
        indirectAmmoInfos_.ReadArchive( input >> xml::start( "indirect-fires" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::AmmoCategoryInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" );
    CategoryInfo::WriteContent( output );
    output << xml::attribute( "type", nType_.Convert() );
    if( bIED_.GetData() == true )
        output << xml::attribute( "ied", bIED_.GetData() );
    if( bIlluminating_.GetData() == true )
    {
        output << xml::start( "illuminating" )
                << xml::attribute( "range", fRange_.GetData() )
                << xml::attribute( "maintain", bMaintainIllumination_.GetData() )
                << xml::end;
    }
    if( bGuided_.GetData() == true )
    {
        output << xml::start( "guided" )
                << xml::attribute( "maintain", bMaintainGuidance_.GetData() )
                << xml::attribute( "range", fGuidanceRange_.GetData() )
               << xml::end;
    }
    if( bDirect_.GetData() == true )
    {
        output << xml::start( "attritions" );
        for( auto itAttrition = attritions_.begin(); itAttrition != attritions_.end(); ++itAttrition )
            (*itAttrition)->WriteArchive( output );
        output << xml::end;
    }
    if( HasUrbanAttrition() )
    {
        output << xml::start( "urban-modifiers" );
        for( auto itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
            (*itUrbanAttrition)->WriteArchive( output );
        output << xml::end;
    }

    if( bIndirect_.GetData() == true )
        indirectAmmoInfos_.WriteArchive( output );

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Resources_Data::AmmoCategoryInfo::Initialize()
{
    indirectAmmoInfos_.Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::AmmoCategoryInfo::HasUrbanAttrition
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
bool ADN_Resources_Data::AmmoCategoryInfo::HasUrbanAttrition() const
{
    for( auto itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
        if( (*itUrbanAttrition)->rCoeff_.GetData() != 0 )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ResourceInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::ResourceInfos::ResourceInfos( sword::DotationCategory nType, T_CategoryInfos_Vector& networkUsableResources )
    : ADN_RefWithName( ENT_Tr::ConvertFromDotationCategory( nType ) )
    , nType_( nType )
    , categories_( true )
    , networkUsableResources_( &networkUsableResources )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::~ResourceInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Resources_Data::ResourceInfos::~ResourceInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::GetCategories
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_Resources_Data::T_CategoryInfos_Vector& ADN_Resources_Data::ResourceInfos::GetCategories()
{
    return categories_;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::FindCategory
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo* ADN_Resources_Data::ResourceInfos::FindCategory( const std::string& strName )
{
    auto it = std::find_if( categories_.begin(), categories_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == categories_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::ReadArchive( xml::xistream& input, kernel::XmlTranslations& /*xmlTranslations*/ )
{
    std::auto_ptr< CategoryInfo > spNew;
    if( strName_.GetData() == "munition" || strName_.GetData() == "explosif" || strName_.GetData() == "mine" )
        spNew.reset( new AmmoCategoryInfo( *this, input.attribute< unsigned int >( "id" ) ) );
    else
        spNew.reset( new CategoryInfo( *this, input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    categories_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::WriteArchive( xml::xostream& output ) const
{
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
        (*it)->WriteArchive( output );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::Initialize()
{
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
        (*it)->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::CheckDatabaseValidity
// Created: JSR 2013-04-18
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
    {
        ( *it )->ptrResourceNature_.CheckValidity( checker, strName_.GetData(), eResources, -1, tools::translate( "ADN_Resources_Data", "Nature" ).toStdString() );
        ( *it )->ptrLogisticSupplyClass_.CheckValidity( checker, strName_.GetData(), eResources, -1, tools::translate( "ADN_Resources_Data", "Logistic supply class" ).toStdString() );
        if( AmmoCategoryInfo* ammoCategory = dynamic_cast< AmmoCategoryInfo* >( *it ) )
            if( ammoCategory->bIndirect_.GetData() && ammoCategory->indirectAmmoInfos_.bEffect_.GetData() )
                ammoCategory->indirectAmmoInfos_.objectType_.CheckValidity( checker, strName_.GetData(), eResources, -1, tools::translate( "ADN_Resources_Data", "Created object" ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::NotifyDestroyed
// Created: JSR 2014-03-19
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::NotifyDestroyed( CategoryInfo* info )
{
    if( networkUsableResources_ )
        networkUsableResources_->RemItem( info );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::AddUsableInNetwork
// Created: JSR 2014-03-19
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::AddUsableInNetwork( CategoryInfo* info )
{
    if( networkUsableResources_ )
        networkUsableResources_->AddItem( info );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::RemoveNetworkUsableResources
// Created: JSR 2014-03-19
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ResourceInfos::RemoveNetworkUsableResources()
{
    networkUsableResources_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data constructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::ADN_Resources_Data()
    : ADN_Data_ABC( eResources )
    , resources_ ()
    , networkUsableResources_( true, false )
{
    for( int n = 0; n < sword::DotationCategory_MAX; ++n )
        resources_.AddItem( new ResourceInfos( static_cast< sword::DotationCategory >( n ), networkUsableResources_ ) );
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        ( *it )->categories_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data destructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Resources_Data::~ADN_Resources_Data()
{
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        ( *it )->RemoveNetworkUsableResources();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::FilesNeeded
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szEquipements_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ReadResource( xml::xistream& input )
{
    std::string category = input.attribute< std::string >( "category" );
    auto nResourceType = ENT_Tr::ConvertToDotationCategory( category );
    if( nResourceType == -1 )
        throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Equipment - Invalid resource type '%1'" ).arg( category.c_str() ).toStdString() );
    resources_.at( nResourceType )->ReadArchive( input, *translations_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "resources" )
            >> xml::list( "resource", *this, &ADN_Resources_Data::ReadResource )
          >> xml::end;

    // Update network usable resources
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        T_CategoryInfos_Vector& categories = ( *it )->categories_;
        for( auto itCat = categories.begin(); itCat != categories.end(); ++itCat )
        {
            CategoryInfo* category = *itCat;
            if( !category )
                continue;
            if( category->bNetworkUsable_.GetData() == true )
                networkUsableResources_.AddItem( category );
        }
    }
    resources_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::WriteArchive( xml::xostream& output ) const
{
    if( resources_.GetErrorStatus() == eError )
    {
        for( auto it = resources_.begin(); it != resources_.end(); ++it )
            if( ( *it )->categories_.GetErrorStatus() == eError )
                throw MASA_EXCEPTION( tools::translate( "ADN_Resources_Data", "Invalid data on tab '%1', subtab '%2'" )
                                      .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( ENT_Tr::ConvertFromDotationCategory( ( *it )->nType_, ENT_Tr::eToTr ).c_str() ).toStdString() );
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );
    }

    output << xml::start( "resources" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Resources" );
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Resources_Data::Initialize()
{
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        (*it)->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::FindResourceCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo* ADN_Resources_Data::FindResourceCategory( const std::string& strDotationName, const std::string& strCategoryName )
{
    auto it = std::find_if( resources_.begin(), resources_.end(), ADN_Tools::NameCmp( strDotationName ) );
    if( it == resources_.end() )
        return 0;
    return (*it)->FindCategory( strCategoryName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::FindResourceCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Resources_Data::CategoryInfo* ADN_Resources_Data::FindResourceCategory( const std::string& strCategoryName )
{
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        ADN_Resources_Data::CategoryInfo* category = (*it)->FindCategory( strCategoryName );
        if( category )
            return category;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResource
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Resources_Data::ResourceInfos& ADN_Resources_Data::GetResource( sword::DotationCategory nType )
{
    return *resources_[ nType ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResourcesThatUse
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
QStringList ADN_Resources_Data::GetResourcesThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( auto itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( AmmoCategoryInfo* ammoCategory = dynamic_cast< AmmoCategoryInfo* >( *itCategory ) )
                if( ammoCategory->bIndirect_.GetData() && ammoCategory->indirectAmmoInfos_.bEffect_.GetData()
                 && ammoCategory->indirectAmmoInfos_.objectType_.GetData()
                 && ammoCategory->indirectAmmoInfos_.objectType_.GetData()->strName_.GetData() == object.strName_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResourcesThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Resources_Data::GetResourcesThatUse( ADN_Natures_Data::NatureInfos& object )
{
    QStringList result;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( auto itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
        {
            ADN_Natures_Data::NatureInfos* infos = ( *itCategory )->ptrResourceNature_.GetData();
            if( infos && infos->strName_.GetData() == object.strName_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResourcesThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Resources_Data::GetResourcesThatUse( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass& object )
{
    QStringList result;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( auto itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
        {
            ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* infos = ( *itCategory )->ptrLogisticSupplyClass_.GetData();
            if( infos && infos->strName_.GetData() == object.strName_.GetData() )
                result << ( *itCategory )->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResourcesWithDirectFire
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Resources_Data::GetResourcesWithDirectFire()
{
    QStringList result;
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( auto itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( AmmoCategoryInfo* ammoCategory = dynamic_cast< AmmoCategoryInfo* >( *itCategory ) )
                if( ammoCategory->bDirect_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResourcesThatUse
// Created: ABR 2012-08-06
// -----------------------------------------------------------------------------
QStringList ADN_Resources_Data::GetResourcesThatUse( ADN_Natures_Data::NatureInfos& object, sword::DotationCategory familly )
{
    QStringList result;
    ResourceInfos& resourceInfos = GetResource( familly );
    for( auto it = resourceInfos.categories_.begin(); it != resourceInfos.categories_.end(); ++it )
    {
        ADN_Natures_Data::NatureInfos* infos = ( *it )->ptrResourceNature_.GetData();
        if(infos && infos->strName_.GetData() == object.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::IsMineOrExplosive
// Created: ABR 2012-11-14
// -----------------------------------------------------------------------------
bool ADN_Resources_Data::IsMineOrExplosive( sword::DotationCategory type )
{
    return type == sword::category_mine || type == sword::category_explosive;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::CheckValidity
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_Resources_Data::T_ResourceInfos_Vector::CheckValidity()
{
    ADN_Ref_ABC::CheckValidity();
    ADN_ErrorStatus errorStatus = GetErrorStatus();

    // Vectors internal validity
    for( auto it = begin(); it != end(); ++it )
    {
        ( *it )->categories_.CheckValidity();
        errorStatus = std::max< ADN_ErrorStatus >( ( *it )->categories_.GetErrorStatus(), errorStatus );
    }

    // Vectors pair validity
    for( auto leftVector = begin(); leftVector != end(); ++leftVector )
    {
        for( auto leftElement = ( *leftVector )->categories_.begin(); leftElement != ( *leftVector )->categories_.end(); ++leftElement )
            for( auto rightVector = leftVector + 1; rightVector != end(); ++rightVector )
                for( auto checker = ( *rightVector )->categories_.checkers_.begin(); checker != ( *rightVector )->categories_.checkers_.end(); ++checker ) // use the right vector because AmmoCategoryInfo inherit CategoryInfo
                    for( auto rightElement = ( *rightVector )->categories_.begin(); rightElement != ( *rightVector )->categories_.end(); ++rightElement )
                        if( *leftElement != *rightElement && !( *checker )->IsValid( **leftElement, **rightElement ) )
                        {
                            ( *checker )->InvalidData( **leftElement, **rightElement );
                            ( *leftVector )->categories_.SetErrorStatus( std::max< ADN_ErrorStatus >( ( *checker )->status_, ( *leftVector )->categories_.GetErrorStatus() ), ( *checker )->msg_ );
                            ( *rightVector )->categories_.SetErrorStatus( std::max< ADN_ErrorStatus >( ( *checker )->status_, ( *rightVector )->categories_.GetErrorStatus() ), ( *checker )->msg_ );
                            errorStatus = std::max< ADN_ErrorStatus >( ( *checker )->status_, errorStatus );
                        }
    }

    SetErrorStatus( errorStatus );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::CheckDatabaseValidity
// Created: JSR 2013-04-18
// -----------------------------------------------------------------------------
void ADN_Resources_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker );
}
