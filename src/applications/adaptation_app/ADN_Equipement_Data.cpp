// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Tools.h"
#include "ADN_Project_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Tr.h"
#include "ADN_DataException.h"
#include "ADN_AttritionInfos.h"
#include "ENT/ENT_Tr.h"
#include <memory>

ADN_Equipement_Data::ResourceInfos* gpDummyDotationInfos;
tools::IdManager ADN_Equipement_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::CategoryInfo
/*
*   This horrible constructor is necessary due to the ADN_Wizard_FirstPage_Default
*   but is never used.
*/
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo()
    : nId_( 0 )
    , parentResource_( *gpDummyDotationInfos )
    , ptrResourceNature_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetLogisticSupplyClasses(), 0 )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CategoryInfo
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo( ResourceInfos& parentDotation )
    : nId_                   ( ADN_Equipement_Data::idManager_.GetNextId() )
    , parentResource_        ( parentDotation )
    , category_              ( ENT_Tr::ConvertFromDotationFamily( parentDotation.nType_ ) )
    , strCodeEMAT6_          ()
    , strCodeEMAT8_          ()
    , strCodeLFRIL_          ()
    , strCodeNNO_            ()
    , ptrResourceNature_     ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetLogisticSupplyClasses(), 0 )
    , rNbrInPackage_         ( 1. )
    , rPackageVolume_        ( 1. )
    , rPackageWeight_        ( 1. )
    , bNetworkUsable_        ( false )
{
    BindExistenceTo( &ptrResourceNature_ );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CategoryInfo
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo( ResourceInfos& parentDotation, unsigned int id )
    : nId_                   ( id )
    , parentResource_        ( parentDotation )
    , category_              ( ENT_Tr::ConvertFromDotationFamily( parentDotation.nType_ ) )
    , strCodeEMAT6_          ()
    , strCodeEMAT8_          ()
    , strCodeLFRIL_          ()
    , strCodeNNO_            ()
    , ptrResourceNature_     ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetLogisticSupplyClasses(), 0 )
    , rNbrInPackage_         ( 1. )
    , rPackageVolume_        ( 1. )
    , rPackageWeight_        ( 1. )
    , bNetworkUsable_        ( false )
{
    BindExistenceTo( &ptrResourceNature_ );
    ADN_Equipement_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CreateCopy
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::CategoryInfo::CreateCopy()
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
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_   = strName_.GetData();
    input >> xml::attribute( "category", category_ );
    std::string dotationNature, logisticSupplyClass;
    input >> xml::optional >> xml::attribute( "codeEMAT6", strCodeEMAT6_ )
          >> xml::optional >> xml::attribute( "codeEMAT8", strCodeEMAT8_ )
          >> xml::optional >> xml::attribute( "codeLFRIL", strCodeLFRIL_ )
          >> xml::optional >> xml::attribute( "codeNNO", strCodeNNO_ )
          >> xml::attribute( "package-size", rNbrInPackage_ )
          >> xml::attribute( "package-mass", rPackageWeight_ )
          >> xml::attribute( "package-volume", rPackageVolume_ )
          >> xml::attribute( "nature", dotationNature )
          >> xml::attribute( "logistic-supply-class", logisticSupplyClass )
          >> xml::attribute( "network-usable", bNetworkUsable_ );
    helpers::ResourceNatureInfos* pNature = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindDotationNature( dotationNature );
    if( !pNature )
        throw ADN_DataException( tools::translate( "Equipment_Data", "Invalid data" ).toStdString(), tools::translate( "Equipment_Data", "Equipment - Invalid resource nature '%1'" ).arg( dotationNature.c_str() ).toStdString() );
    ptrResourceNature_ = pNature;
    helpers::LogisticSupplyClass* pClass = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindLogisticSupplyClass( logisticSupplyClass );
    if( !pClass )
        throw ADN_DataException( tools::translate( "Equipment_Data", "Invalid data" ).toStdString(), tools::translate( "Equipment_Data", "Equipment - Invalid resource logistic supply class '%1'" ).arg( logisticSupplyClass.c_str() ).toStdString() );
    ptrLogisticSupplyClass_ = pClass;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" );
    WriteContent( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::Initialize()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data:::CategoryInfo::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteContent( xml::xostream& output ) const
{
    output << xml::attribute( "category", category_ )
           << xml::attribute( "name", strName_ )
           << xml::attribute( "id", nId_ )
           << xml::attribute( "package-size", rNbrInPackage_ )
           << xml::attribute( "package-mass", rPackageWeight_ )
           << xml::attribute( "package-volume", rPackageVolume_ )
           << xml::attribute( "nature", ptrResourceNature_.GetData()->strName_.GetData() )
           << xml::attribute( "logistic-supply-class", ptrLogisticSupplyClass_.GetData()->strName_.GetData() )
           << xml::attribute( "id-nature", ptrResourceNature_.GetData()->nId_ )
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
ADN_Equipement_Data::ModificatorPostureInfos::ModificatorPostureInfos( const E_UnitPosture& e )
    : eType_ ( e )
    , rCoeff_(1.0)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ModificatorPostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ModificatorPostureInfos::WriteArchive( xml::xostream& output ) const
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
ADN_Equipement_Data::IndirectAmmoInfos::IndirectAmmoInfos()
    : bExplosive_          ( false )
    , bSmoke_              ( false )
    , bFlare_              ( false )
    , bMine_               ( false )
    , bEffect_             ( false )
    , nIntervention_       ( 1 )
    , rDispersionX_        ( 1.0 )
    , rDispersionY_        ( 1.0 )
    , rNeutralizationRatio_( 1 )
    , rDispersionFactor_   ( 0 )
    , vModifStance_        ()
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
// Name: IndirectAmmoInfos::CopyFrom
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::CopyFrom( ADN_Equipement_Data::IndirectAmmoInfos& ammoInfos )
{
    bExplosive_ = ammoInfos.bExplosive_.GetData();
    bSmoke_ = ammoInfos.bSmoke_.GetData();
    bFlare_ = ammoInfos.bFlare_.GetData();
    bMine_ = ammoInfos.bMine_.GetData();
    bEffect_ = ammoInfos.bEffect_.GetData();

    rDispersionX_ = ammoInfos.rDispersionX_.GetData();
    rDispersionY_ = ammoInfos.rDispersionY_.GetData();
    rNeutralizationRatio_ = ammoInfos.rNeutralizationRatio_.GetData();
    rDispersionFactor_ = ammoInfos.rDispersionFactor_.GetData();

    for( uint i=0 ; i< eNbrUnitPosture ; ++i)
        vModifStance_[i]->rCoeff_ = ammoInfos.vModifStance_[ i ]->rCoeff_.GetData();

    flareDeployTime_ = ammoInfos.flareDeployTime_.GetData();
    flareLifeTime_ = ammoInfos.flareLifeTime_.GetData();
    smokeDeployTime_ = ammoInfos.smokeDeployTime_.GetData();
    smokeLifeTime_ = ammoInfos.smokeLifeTime_.GetData();
    nMineNumber_ = ammoInfos.nMineNumber_.GetData();
    objectType_ = ammoInfos.objectType_.GetData();
    effectLifeTime_ = ammoInfos.effectLifeTime_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::IndirectAmmoInfos::ReadPh
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::ReadPh( xml::xistream& input )
{
    std::string posture;
    input >> xml::attribute( "target-posture", posture );
    for( int i = 0; i < eNbrUnitPosture; ++i )
        if( ADN_Tools::ComputePostureScriptName( E_UnitPosture( i ) ) == posture )
        {
            vModifStance_[i]->ReadArchive( input );
            return;
        }
    throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid stance '%1'" ).arg( posture.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "intervention-type", nIntervention_ )
          >> xml::attribute( "x-dispersion", rDispersionX_ )
          >> xml::attribute( "y-dispersion", rDispersionY_ )
          >> xml::list( "indirect-fire", *this, &ADN_Equipement_Data::IndirectAmmoInfos::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadIndirectFire
// Created: LGY 2012-02-24
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::ReadIndirectFire( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    E_TypeMunitionTirIndirect indirect = ADN_Tr::ConvertToTypeMunitionTirIndirect( type );
    if( indirect == (E_TypeMunitionTirIndirect)-1 )
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid indirect fire ammo type '%1'" ).arg( type.c_str() ).toStdString() );

    switch( indirect )
    {
        case eTypeMunitionTirIndirect_Explosif:
            bExplosive_ = true;
            input >> xml::attribute( "neutralization-ratio", rNeutralizationRatio_ )
                  >> xml::attribute( "dispersion-factor", rDispersionFactor_ )
                  >> xml::list( "ph", *this, &ADN_Equipement_Data::IndirectAmmoInfos::ReadPh );
            break;
        case eTypeMunitionTirIndirect_Fumigene:
            bSmoke_ = true;
            input >> xml::attribute( "setup-time", smokeDeployTime_ )
                  >> xml::attribute( "life-time", smokeLifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Eclairant:
            bFlare_ = true;
            input >> xml::attribute( "setup-time", flareDeployTime_ )
                  >> xml::attribute( "life-time", flareLifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Mine:
            bMine_ = true;
            input >> xml::attribute( "mine-count", nMineNumber_ );
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
void ADN_Equipement_Data::IndirectAmmoInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "indirect-fires" )
                << xml::attribute( "intervention-type", nIntervention_ )
                << xml::attribute( "x-dispersion", rDispersionX_ )
                << xml::attribute( "y-dispersion", rDispersionY_ );

    if( bExplosive_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Explosif ) )
                    << xml::attribute( "neutralization-ratio", rNeutralizationRatio_ )
                    << xml::attribute( "dispersion-factor", rDispersionFactor_ );
        for( CIT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
            ( *it )->WriteArchive( output );
        output << xml::end;
    }
    if( bSmoke_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Fumigene ) )
                    << xml::attribute( "setup-time", smokeDeployTime_ )
                    << xml::attribute( "life-time", smokeLifeTime_ )
                << xml::end;
    }
    if( bFlare_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Eclairant ) )
                    << xml::attribute( "setup-time", flareDeployTime_ )
                    << xml::attribute( "life-time", flareLifeTime_ )
                << xml::end;
    }
    if( bMine_.GetData() )
    {
        output << xml::start( "indirect-fire" )
                    << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( eTypeMunitionTirIndirect_Mine ) )
                    << xml::attribute( "mine-count", nMineNumber_ )
                << xml::end;
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
void ADN_Equipement_Data::IndirectAmmoInfos::Initialize()
{
    if( objectType_.GetData() != 0 )
        return;
    if( !strObjectType_.empty() )
    {
        ADN_Objects_Data_ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( strObjectType_ );
        if( pObject )
            objectType_ = pObject;
        else
            QMessageBox::warning( QApplication::activeWindow(), tools::translate( "Equipment_Data", "Invalid data" ), tools::translate( "Equipment_Data", "Object type - Invalid object '%1'" ).arg( strObjectType_.c_str() ), QMessageBox::Ok, Qt::NoButton );
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
ADN_Equipement_Data::AmmoCategoryInfo::AmmoCategoryInfo( ResourceInfos& parentDotation )
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
    , attritions_           ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
    , modifUrbanBlocks_     ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    BindExistenceTo( &( indirectAmmoInfos_.objectType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::AmmoCategoryInfo
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::AmmoCategoryInfo::AmmoCategoryInfo( ResourceInfos& parentDotation, unsigned int id )
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
    , attritions_           ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
    , modifUrbanBlocks_     ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    BindExistenceTo( &( indirectAmmoInfos_.objectType_ ) );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::CreateCopy
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::AmmoCategoryInfo::CreateCopy()
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
// Name: ADN_Equipement_Data::AmmoCategoryInfo::ReadAttrition
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::ReadAttrition( xml::xistream& input )
{
    bDirect_ = true;
    std::string protection = input.attribute< std::string >( "protection" );
    helpers::IT_AttritionInfos_Vector itAttrition = std::find_if( attritions_.begin(), attritions_.end(), helpers::AttritionInfos::Cmp( protection ));
    if( itAttrition == attritions_.end() )
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).toStdString() );
    (*itAttrition)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::AmmoCategoryInfo::ReadUrbanModifer
// Created: SLG 2010-04-13
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::ReadUrbanModifer( xml::xistream& input )
{
    std::string material = input.attribute< std::string >( "material-type" );
    helpers::IT_UrbanAttritionInfos_Vector it = std::find_if( modifUrbanBlocks_.begin(), modifUrbanBlocks_.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
    if( it == modifUrbanBlocks_.end() )
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid urban Material type '%1'" ).arg( material.c_str() ).toStdString() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::ReadArchive( xml::xistream& input )
{
    CategoryInfo::ReadArchive( input );

    std::string type;
    input >> xml::optional >> xml::attribute( "type", type );
    input >> xml::optional >> xml::attribute( "ied", bIED_ );
    if( !type.empty() )
    {
        nType_ = ENT_Tr::ConvertToAmmunitionType( type );
        if( nType_ == E_AmmunitionType( -1 ) )
            throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid resource type '%1'" ).arg( type.c_str() ).toStdString() );
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
            >> xml::list( "attrition", *this, &ADN_Equipement_Data::AmmoCategoryInfo::ReadAttrition )
          >> xml::end
          >> xml::optional
          >> xml::start( "urban-modifiers" )
            >> xml::list( "urban-modifier", *this, &ADN_Equipement_Data::AmmoCategoryInfo::ReadUrbanModifer )
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
void ADN_Equipement_Data::AmmoCategoryInfo::WriteArchive( xml::xostream& output ) const
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
        for( helpers::CIT_AttritionInfos_Vector itAttrition = attritions_.begin(); itAttrition != attritions_.end(); ++itAttrition )
            (*itAttrition)->WriteArchive( output );
        output << xml::end;
    }
    if( HasUrbanAttrition() )
    {
        output << xml::start( "urban-modifiers" );
        for( helpers::CIT_UrbanAttritionInfos_Vector itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
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
void ADN_Equipement_Data::AmmoCategoryInfo::Initialize()
{
    indirectAmmoInfos_.Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::AmmoCategoryInfo::HasUrbanAttrition
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
bool ADN_Equipement_Data::AmmoCategoryInfo::HasUrbanAttrition() const
{
    for( helpers::CIT_UrbanAttritionInfos_Vector itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
        if( (*itUrbanAttrition)->rCoeff_.GetData() != 0 )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ResourceInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ResourceInfos::ResourceInfos( E_DotationFamily nType )
    : ADN_RefWithName( ENT_Tr::ConvertFromDotationFamily( nType ) )
    , nType_( nType )
    , categories_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::~ResourceInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ResourceInfos::~ResourceInfos()
{
    this->Reset();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::Reset
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ResourceInfos::Reset()
{
    categories_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::GetCategories
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_Equipement_Data::T_CategoryInfos_Vector& ADN_Equipement_Data::ResourceInfos::GetCategories()
{
    return categories_;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::FindCategory
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::ResourceInfos::FindCategory( const std::string& strName )
{
    IT_CategoryInfos_Vector it = std::find_if( categories_.begin(), categories_.end(), ADN_Tools::NameCmp<CategoryInfo>( strName ) );
    if( it == categories_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ResourceInfos::ReadArchive( xml::xistream& input )
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
void ADN_Equipement_Data::ResourceInfos::WriteArchive( xml::xostream& output ) const
{
    for( CIT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
        (*it)->WriteArchive( output );
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ResourceInfos::Initialize()
{
    for( CIT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
        (*it)->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data constructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ADN_Equipement_Data()
    : ADN_Data_ABC()
    , resources_ ()
{
    for( int n = 0; n < eNbrDotationFamily; ++n )
        resources_.AddItem( new ResourceInfos( (E_DotationFamily)n ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data destructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::~ADN_Equipement_Data()
{
    resources_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FilesNeeded
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::FilesNeeded(T_StringList& files ) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szEquipements_.GetData());
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::Reset
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::Reset()
{
    idManager_.Reset();
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
        (*it)->Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ReadResource( xml::xistream& input )
{
    std::string category;
    input >> xml::attribute( "category", category );
    E_DotationFamily nResourceType = ENT_Tr::ConvertToDotationFamily( category );
    if( nResourceType == -1 )
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid resource type '%1'" ).arg( category.c_str() ).toStdString() );
    resources_.at( nResourceType )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "resources" )
            >> xml::list( "resource", *this, &ADN_Equipement_Data::ReadResource )
          >> xml::end;

    // Update network usable resources
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        T_CategoryInfos_Vector& categories = ( *it )->categories_;
        for( IT_CategoryInfos_Vector itCat = categories.begin(); itCat != categories.end(); ++itCat )
        {
            CategoryInfo* category = *itCat;
            if( !category )
                continue;
            if( category->bNetworkUsable_.GetData() == true )
                networkUsableResources_.AddItem( category );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "resources" );
    ADN_Tools::AddSchema( output, "Resources" );
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::Initialize()
{
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
        (*it)->Initialize();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FindEquipementCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::FindEquipementCategory( const std::string& strDotationName, const std::string& strCategoryName )
{
    IT_ResourceInfos_Vector it = std::find_if( resources_.begin(), resources_.end(), ADN_Tools::NameCmp<ResourceInfos>( strDotationName ) );
    if( it == resources_.end() )
        return 0;
    return (*it)->FindCategory( strCategoryName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FindEquipementCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::FindEquipementCategory( const std::string& strCategoryName )
{
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ADN_Equipement_Data::CategoryInfo* category = (*it)->FindCategory( strCategoryName );
        if( category )
            return category;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetDotation
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ResourceInfos& ADN_Equipement_Data::GetDotation( E_DotationFamily nType )
{
    return *resources_[ nType ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetEquipmentsThatUse
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
QStringList ADN_Equipement_Data::GetEquipmentsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( CIT_CategoryInfos_Vector itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( AmmoCategoryInfo* ammoCategory = dynamic_cast< AmmoCategoryInfo* >( *itCategory ) )
                if( ammoCategory->bIndirect_.GetData() && ammoCategory->indirectAmmoInfos_.objectType_.GetData()->strType_.GetData() == object.strType_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetEquipmentsThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Equipement_Data::GetEquipmentsThatUse( helpers::ResourceNatureInfos& object )
{
    QStringList result;
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( CIT_CategoryInfos_Vector itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( ( *itCategory )->ptrResourceNature_.GetData()->strName_.GetData() == object.strName_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetEquipmentsThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Equipement_Data::GetEquipmentsThatUse( helpers::LogisticSupplyClass& object )
{
    QStringList result;
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( CIT_CategoryInfos_Vector itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( ( *itCategory )->ptrLogisticSupplyClass_.GetData()->strName_.GetData() == object.strName_.GetData() )
                result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetEquipmentsWithDirectFire
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Equipement_Data::GetEquipmentsWithDirectFire()
{
    QStringList result;
    for( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ResourceInfos* pComp = *it;
        for( CIT_CategoryInfos_Vector itCategory = pComp->categories_.begin(); itCategory != pComp->categories_.end(); ++itCategory )
            if( AmmoCategoryInfo* ammoCategory = dynamic_cast< AmmoCategoryInfo* >( *itCategory ) )
                if( ammoCategory->bDirect_.GetData() )
                    result << ( *itCategory )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetEquipmentsThatUse
// Created: ABR 2012-08-06
// -----------------------------------------------------------------------------
QStringList ADN_Equipement_Data::GetEquipmentsThatUse( helpers::ResourceNatureInfos& object, E_DotationFamily familly )
{
    QStringList result;
    ResourceInfos& resourceInfos = GetDotation( familly );
    for( CIT_CategoryInfos_Vector it = resourceInfos.categories_.begin(); it != resourceInfos.categories_.end(); ++it )
        if( ( *it )->ptrResourceNature_.GetData()->strName_.GetData() == object.strName_.GetData() )
                    result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::IsMineOrExplosive
// Created: ABR 2012-11-14
// -----------------------------------------------------------------------------
bool ADN_Equipement_Data::IsMineOrExplosive( E_DotationFamily type )
{
    return type == eDotationFamily_Mine || type == eDotationFamily_Explosif;
}
