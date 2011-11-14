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
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tr.h"
#include "ADN_DataException.h"
#include "ADN_AttritionInfos.h"
#include "ENT/ENT_Tr.h"
#include <memory>

ADN_Equipement_Data::ResourceInfos* gpDummyDotationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::CategoryInfo
/*
*   This horrible constructor is necessary due to the ADN_Wizard_FirstPage_Default
*   but is never used.
*/
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo()
    : ADN_Ref_ABC         ( "ADN_Equipement_Data::CategoryInfo" )
    , ADN_DataTreeNode_ABC()
    , parentResource_   ( *gpDummyDotationInfos )
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
    : ADN_Ref_ABC         ( "ADN_Equipement_Data::CategoryInfo" )
    , ADN_DataTreeNode_ABC()
    , parentResource_   ( parentDotation )
    , strName_          ()
    , category_         ( ENT_Tr::ConvertFromDotationFamily( parentDotation.nType_ ) )
    , nMosId_           ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetNextCatId() )
    , strCodeEMAT6_     ()
    , strCodeEMAT8_     ()
    , strCodeLFRIL_     ()
    , strCodeNNO_       ()
    , ptrResourceNature_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
    , ptrLogisticSupplyClass_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetLogisticSupplyClasses(), 0 )
    , rNbrInPackage_    ( 1. )
    , rPackageVolume_   ( 1. )
    , rPackageWeight_   ( 1. )
{
    strName_.SetDataName( "le nom d'" );
    strName_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::GetNodeName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::CategoryInfo::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::GetItemName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::CategoryInfo::GetItemName()
{
    return std::string();
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
          >> xml::attribute( "logistic-supply-class", logisticSupplyClass );
    helpers::ResourceNatureInfos* pNature = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindDotationNature( dotationNature );
    if( !pNature )
        throw ADN_DataException( tools::translate( "Equipment_Data", "Invalid data" ).ascii(), tools::translate( "Equipment_Data", "Equipment - Invalid resource nature '%1'" ).arg( dotationNature.c_str() ).ascii() );
    ptrResourceNature_ = pNature;
    helpers::LogisticSupplyClass* pClass = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindLogisticSupplyClass( logisticSupplyClass );
    if( !pClass )
        throw ADN_DataException( tools::translate( "Equipment_Data", "Invalid data" ).ascii(), tools::translate( "Equipment_Data", "Equipment - Invalid resource logistic supply class '%1'" ).arg( logisticSupplyClass.c_str() ).ascii() );
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
// Name: ADN_Equipement_Data:::CategoryInfo::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteContent( xml::xostream& output ) const
{
    output << xml::attribute( "category", category_ )
           << xml::attribute( "name", strName_ )
           << xml::attribute( "id", nMosId_ )
           << xml::attribute( "package-size", rNbrInPackage_ )
           << xml::attribute( "package-mass", rPackageWeight_ )
           << xml::attribute( "package-volume", rPackageVolume_ )
           << xml::attribute( "nature", ptrResourceNature_.GetData()->GetData() )
           << xml::attribute( "logistic-supply-class", ptrLogisticSupplyClass_.GetData()->GetData() )
           << xml::attribute( "id-nature", ptrResourceNature_.GetData()->GetId() )
           << xml::attribute( "codeEMAT6", strCodeEMAT6_ )
           << xml::attribute( "codeEMAT8", strCodeEMAT8_ )
           << xml::attribute( "codeLFRIL", strCodeLFRIL_ )
           << xml::attribute( "codeNNO", strCodeNNO_ );
}

//-----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ModificatorPostureInfos
// Created: JDY 03-09-29
//-----------------------------------------------------------------------------
ADN_Equipement_Data::ModificatorPostureInfos::ModificatorPostureInfos( const E_UnitPosture& e )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , eType_ ( e )
    , rCoeff_(1.0)
{
    rCoeff_.SetDataName( "le modificateur de dégats selon la posture " );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::ModificatorPostureInfos::GetNodeName()
{
    return ENT_Tr::ConvertFromUnitPosture( eType_,  ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::ModificatorPostureInfos::GetItemName()
{
    return ENT_Tr::ConvertFromUnitPosture( eType_,  ENT_Tr_ABC::eToTr );
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
    : nIndirectType_       ( (E_TypeMunitionTirIndirect)0 )
    , nIntervention_       ( 0 )
    , rDispersionX_        ( 0.0 )
    , rDispersionY_        ( 0.0 )
    , rNeutralizationRatio_( 1 )
    , vModifStance_        ()
    , deployTime_          ( "0s" )
    , flareLifeTime_       ( "0s" )
    , nMineNumber_         ( 0 )
    , effectLifeTime_      ( "0s" )
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
    nIndirectType_ = ammoInfos.nIndirectType_.GetData();
    nIntervention_ = ammoInfos.nIntervention_.GetData();
    rDispersionX_ = ammoInfos.rDispersionX_.GetData();
    rDispersionY_ = ammoInfos.rDispersionY_.GetData();
    rNeutralizationRatio_ = ammoInfos.rNeutralizationRatio_.GetData();

    for( uint i=0 ; i< eNbrUnitPosture ; ++i)
        vModifStance_[i]->rCoeff_ = ammoInfos.vModifStance_[ i ]->rCoeff_.GetData();

    deployTime_ = ammoInfos.deployTime_.GetData();
    flareLifeTime_ = ammoInfos.flareLifeTime_.GetData();
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
    throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid stance '%1'" ).arg( posture.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type )
          >> xml::attribute( "intervention-type", nIntervention_ )
          >> xml::attribute( "x-dispersion", rDispersionX_ )
          >> xml::attribute( "y-dispersion", rDispersionY_ );
    nIndirectType_ = ADN_Tr::ConvertToTypeMunitionTirIndirect( type );
    if( nIndirectType_ == (E_TypeMunitionTirIndirect)-1 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid indirect fire ammo type '%1'" ).arg( type.c_str() ).ascii() );

    switch( nIndirectType_.GetData() )
    {
        case eTypeMunitionTirIndirect_Explosif:
        case eTypeMunitionTirIndirect_Aced:
        case eTypeMunitionTirIndirect_Grenade:
            input >> xml::attribute( "neutralization-ratio", rNeutralizationRatio_ )
                  >> xml::list( "ph", *this, &ADN_Equipement_Data::IndirectAmmoInfos::ReadPh );
            break;
        case eTypeMunitionTirIndirect_Fumigene:
        case eTypeMunitionTirIndirect_Eclairant:
            input >> xml::attribute( "setup-time", deployTime_ )
                  >> xml::attribute( "life-time", flareLifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Mine:
            input >> xml::attribute( "mine-count", nMineNumber_ );
            break;
        case eTypeMunitionTirIndirect_Effect:
            input >> xml::attribute( "object-type", objectType_ );
            input >> xml::attribute( "life-time", effectLifeTime_ );
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "indirect-fire" )
            << xml::attribute( "type", ADN_Tr::ConvertFromTypeMunitionTirIndirect( nIndirectType_.GetData() ) )
            << xml::attribute( "intervention-type", nIntervention_ )
            << xml::attribute( "x-dispersion", rDispersionX_ )
            << xml::attribute( "y-dispersion", rDispersionY_ );

    switch( nIndirectType_.GetData() )
    {
    case eTypeMunitionTirIndirect_Explosif:
    case eTypeMunitionTirIndirect_Aced:
    case eTypeMunitionTirIndirect_Grenade:
        output << xml::attribute( "neutralization-ratio", rNeutralizationRatio_ );
        for( CIT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
            ( *it )->WriteArchive( output );
        break;

    case eTypeMunitionTirIndirect_Fumigene:
    case eTypeMunitionTirIndirect_Eclairant:
        output << xml::attribute( "setup-time", deployTime_ )
               << xml::attribute( "life-time", flareLifeTime_ );
        break;

    case eTypeMunitionTirIndirect_Mine:
        output << xml::attribute( "mine-count", nMineNumber_ );
        break;
    case eTypeMunitionTirIndirect_Effect:
        output << xml::attribute( "object-type", objectType_ )
               << xml::attribute( "life-time", effectLifeTime_ );
        break;
    }

    output << xml::end;
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
    // NOTHING
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

    pCopy->bIlluminating_ = bIlluminating_.GetData();
    pCopy->bMaintainIllumination_ = bMaintainIllumination_.GetData();
    pCopy->fRange_ = fRange_.GetData();

    pCopy->bGuided_ = bGuided_.GetData();
    pCopy->bMaintainGuidance_ = bMaintainGuidance_.GetData();
    pCopy->fGuidanceRange_ = fGuidanceRange_.GetData();

    for( uint n = 0; n < attritions_.size(); ++n )
        pCopy->attritions_[n]->CopyFrom( * attritions_[n] );
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
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).ascii() );
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
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid urban Material type '%1'" ).arg( material.c_str() ).ascii() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::AmmoCategoryInfo::ReadIndirectFire
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::ReadIndirectFire( xml::xistream& input )
{
    bIndirect_ = true;
    indirectAmmoInfos_.ReadArchive( input );
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
        if( nType_ == E_MunitionType( -1 ) )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid resource type '%1'" ).arg( type.c_str() ).ascii() );
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
          >> xml::end
          >> xml::list( "indirect-fire", *this, &ADN_Equipement_Data::AmmoCategoryInfo::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "resource" );
    CategoryInfo::WriteContent( output );
    output << xml::attribute( "type", ENT_Tr::ConvertFromAmmunitionType( nType_.GetData() ) );
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
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nType_                 ( nType )
    , strName_               ( ENT_Tr::ConvertFromDotationFamily( nType ) )
    , categories_            ( true, "ADN_Equipement_Data::ResourceInfos::categories_" )
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
// Name: ResourceInfos::GetNodeName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::ResourceInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ResourceInfos::GetItemName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::ResourceInfos::GetItemName()
{
    return std::string();
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
        spNew.reset( new AmmoCategoryInfo( *this ) );
    else
        spNew.reset( new CategoryInfo( *this ) );
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
// Name: ADN_Equipement_Data constructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ADN_Equipement_Data()
    : ADN_Data_ABC()
    , nNextCatId_( 1 )
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
    nNextCatId_ = 1;
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
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid resource type '%1'" ).arg( category.c_str() ).ascii() );
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
    for ( IT_ResourceInfos_Vector it = resources_.begin(); it != resources_.end(); ++it )
    {
        ADN_Equipement_Data::CategoryInfo* category = (*it)->FindCategory( strCategoryName );
        if( category )
            return category;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetNextCatId
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
int ADN_Equipement_Data::GetNextCatId()
{
    return nNextCatId_++;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetDotation
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ResourceInfos& ADN_Equipement_Data::GetDotation( E_DotationFamily nType )
{
    return * resources_[ nType ];
}
