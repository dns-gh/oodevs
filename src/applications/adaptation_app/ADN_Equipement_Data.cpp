// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Data.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 14:16 $
// $Revision: 17 $
// $Workfile: ADN_Equipement_Data.cpp $
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

#include "ENT/ENT_Tr.h"

#include <memory>


ADN_Equipement_Data::DotationInfos* gpDummyDotationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::CategoryInfo
/*
*   This horrible constructor is necessary due to the ADN_Wizard_FirstPage_Default
*   but is never used.
*/
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo()
: ADN_Ref_ABC           ( "ADN_Equipement_Data::CategoryInfo" )
, ADN_DataTreeNode_ABC  ()
, parentDotation_       ( *gpDummyDotationInfos )
, ptrDotationNature_    ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::CategoryInfo
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo::CategoryInfo( DotationInfos& parentDotation )
: ADN_Ref_ABC           ( "ADN_Equipement_Data::CategoryInfo" )
, ADN_DataTreeNode_ABC  ()
, strCodeEMAT6_         ()
, strCodeEMAT8_         ()
, strCodeLFRIL_         ()
, strCodeNNO_           ()
, ptrDotationNature_    ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), 0 )
, parentDotation_       ( parentDotation )
, strName_              ()
, nMosId_               ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetNextCatId() )
, rNbrInPackage_        ( 0. )
, rPackageVolume_       ( 0. )
, rPackageWeight_       ( 0. )
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
    CategoryInfo* pCopy = new CategoryInfo( parentDotation_ );

    pCopy->rNbrInPackage_  = rNbrInPackage_ .GetData();
    pCopy->rPackageVolume_ = rPackageVolume_.GetData();
    pCopy->rPackageWeight_ = rPackageWeight_.GetData();
    pCopy->ptrDotationNature_ = ptrDotationNature_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "category", strName_ );
    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_   = strName_.GetData();
    
    std::string strNature;
    input >> xml::optional() >> xml::attribute( "codeEMAT6", strCodeEMAT6_ )
          >> xml::optional() >> xml::attribute( "codeEMAT8", strCodeEMAT8_ )
          >> xml::optional() >> xml::attribute( "codeLFRIL", strCodeLFRIL_ )
          >> xml::optional() >> xml::attribute( "codeNNO", strCodeNNO_ )
          >> xml::attribute( "package-size", rNbrInPackage_ )
          >> xml::attribute( "package-mass", rPackageWeight_ )
          >> xml::attribute( "package-volume", rPackageVolume_ )
          >> xml::attribute( "nature", strNature );
    ADN_Categories_Data::DotationNatureInfos* pNature = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindDotationNature( strNature );
    if( !pNature )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid resource nature '%1'" ).arg( strNature.c_str() ).ascii() );
    ptrDotationNature_ = pNature;
}

// -----------------------------------------------------------------------------
// Name: CategoryInfo::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "dotation" );
    WriteContent( output );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data:::CategoryInfo::WriteContent
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteContent( xml::xostream& output )
{
    output << xml::attribute( "name", parentDotation_.strName_ )
           << xml::attribute( "category", strName_ )
           << xml::attribute( "id", nMosId_ )
           << xml::attribute( "package-size", rNbrInPackage_ )
           << xml::attribute( "package-mass", rPackageWeight_ )
           << xml::attribute( "package-volume", rPackageVolume_ )
           << xml::attribute( "nature", *ptrDotationNature_.GetData() )
           << xml::attribute( "codeEMAT6", strCodeEMAT6_ )
           << xml::attribute( "codeEMAT8", strCodeEMAT8_ )
           << xml::attribute( "codeLFRIL", strCodeLFRIL_ )
           << xml::attribute( "codeNNO", strCodeNNO_ );
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::AttritionInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::AttritionInfos::AttritionInfos(ADN_Categories_Data::ArmorInfos* ptr)
: ptrArmor_         ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), ptr )
, rDestroy_         ( 0.0 )
, rRepairWithEvac_  ( 0.0 )
, rRepairNoEvac_    ( 0.0 )
{
    this->BindExistenceTo( &ptrArmor_ );
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::GetNodeName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::AttritionInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::GetItemName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::AttritionInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::CopyFrom
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AttritionInfos::CopyFrom( ADN_Equipement_Data::AttritionInfos& attritions )
{
    rDestroy_ = attritions.rDestroy_.GetData();
    rRepairNoEvac_ = attritions.rRepairNoEvac_.GetData();
    rRepairWithEvac_ = attritions.rRepairWithEvac_.GetData();
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AttritionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "destruction", rDestroy_ )
          >> xml::attribute( "repairable-with-evacuation", rRepairWithEvac_ )
          >> xml::attribute( "repairable-without-evacuation", rRepairNoEvac_ );
    rDestroy_        = rDestroy_.GetData() * 100.0;
    rRepairWithEvac_ = rRepairWithEvac_.GetData() * 100.0;
    rRepairNoEvac_   = rRepairNoEvac_.GetData() * 100.0;
}

// -----------------------------------------------------------------------------
// Name: AttritionInfos::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AttritionInfos::WriteArchive( xml::xostream& output, const std::string& tag /*= "attrition"*/ )
{
    output << xml::start( tag )
            << xml::attribute( "protection", ptrArmor_.GetData()->strName_ )
            << xml::attribute( "destruction", rDestroy_.GetData() / 100.0 )
            << xml::attribute( "repairable-with-evacuation", rRepairWithEvac_.GetData() / 100.0 )
            << xml::attribute( "repairable-without-evacuation", rRepairNoEvac_.GetData() / 100.0 )
           << xml::end();
}

//-----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ModificatorPostureInfos
// Created: JDY 03-09-29
//-----------------------------------------------------------------------------
ADN_Equipement_Data::ModificatorPostureInfos::ModificatorPostureInfos(const E_UnitPosture& e )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, eType_(e)
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
    return std::string();
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
void ADN_Equipement_Data::ModificatorPostureInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "ph" )
            << xml::attribute( "target-posture", ADN_Tools::ComputePostureScriptName( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::IndirectAmmoInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::IndirectAmmoInfos::IndirectAmmoInfos()
: nIndirectType_    ( (E_TypeMunitionTirIndirect)0 )
, nIntervention_    ( 0 )
, rDispersionX_     ( 0.0 )
, rDispersionY_     ( 0.0 )
, rNeutralizationRatio_ ( 0 )
, vModifStance_     ()
, deployTime_      ( "0s" )
, lifeTime_        ( "0s" )
, nMineNumber_      ( 0 )
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
    lifeTime_ = ammoInfos.lifeTime_.GetData();
    nMineNumber_ = ammoInfos.nMineNumber_.GetData();
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
                  >> xml::attribute( "life-time", lifeTime_ );
            break;
        case eTypeMunitionTirIndirect_Mine:
            input >> xml::attribute( "mine-count", nMineNumber_ );
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::WriteArchive( xml::xostream& output )
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
               << xml::attribute( "life-time", lifeTime_ );
        break;

    case eTypeMunitionTirIndirect_Mine:
        output << xml::attribute( "mine-count", nMineNumber_ );
        break;
    }

    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::AmmoCategoryInfo
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::AmmoCategoryInfo::AmmoCategoryInfo( DotationInfos& parentDotation )
: CategoryInfo      ( parentDotation )
, bDirect_          ( false )
, bTrancheD_        ( false )
, bIndirect_        ( false )
, attritions_       (ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos())
, indirectAmmoInfos_()
{
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::CreateCopy
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::AmmoCategoryInfo::CreateCopy()
{
    AmmoCategoryInfo* pCopy = new AmmoCategoryInfo( parentDotation_ );
    pCopy->bTrancheD_ = bTrancheD_.GetData();
    pCopy->nType_ = nType_.GetData();
    pCopy->bDirect_ = bDirect_.GetData();
    pCopy->bIndirect_ = bIndirect_.GetData();
    
    pCopy->rNbrInPackage_  = rNbrInPackage_ .GetData();
    pCopy->rPackageVolume_ = rPackageVolume_.GetData();
    pCopy->rPackageWeight_ = rPackageWeight_.GetData();

    pCopy->strCodeEMAT6_  = strCodeEMAT6_.GetData(); 
    pCopy->strCodeEMAT8_  = strCodeEMAT8_.GetData();
    pCopy->strCodeLFRIL_  = strCodeLFRIL_.GetData();
    pCopy->strCodeNNO_    = strCodeNNO_.GetData();

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
    std::string protection = xml::attribute< std::string >( input, "protection" );
    IT_AttritionInfos_Vector itAttrition = std::find_if( attritions_.begin(), attritions_.end(),AttritionInfos::Cmp( protection ));
    if( itAttrition == attritions_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).ascii() );
    (*itAttrition)->ReadArchive( input );
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
    input >> xml::optional() >> xml::attribute( "d-type", bTrancheD_ )
          >> xml::attribute( "type", type );
    nType_ = ADN_Tr::ConvertToMunitionType( type );
    if( nType_ == E_MunitionType( -1 ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid resource type '%1'" ).arg( type.c_str() ).ascii() );

    input >> xml::optional() 
          >> xml::start( "attritions" )
            >> xml::list( "attrition", *this, &ADN_Equipement_Data::AmmoCategoryInfo::ReadAttrition )
          >> xml::end()
          >> xml::list( "indirect-fire", *this, &ADN_Equipement_Data::AmmoCategoryInfo::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "dotation" );
    CategoryInfo::WriteContent( output );
    output << xml::attribute( "type", ADN_Tr::ConvertFromMunitionType( nType_.GetData() ) );
    if( bTrancheD_.GetData() )
        output << xml::attribute( "d-type", bTrancheD_ );
    if( bDirect_.GetData() == true )
    {
        output << xml::start( "attritions" );
        for( IT_AttritionInfos_Vector itAttrition = attritions_.begin(); itAttrition != attritions_.end(); ++itAttrition )
            (*itAttrition)->WriteArchive( output );
        output << xml::end();
    }

    if( bIndirect_.GetData() == true )
        indirectAmmoInfos_.WriteArchive( output );

    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::DotationInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::DotationInfos::DotationInfos( E_DotationFamily nType )
: ADN_Ref_ABC            ()
, ADN_DataTreeNode_ABC   ()
, nType_                 ( nType )
, strName_               ( ENT_Tr::ConvertFromDotationFamily( nType ) )
, categories_            ( true, "ADN_Equipement_Data::DotationInfos::categories_" )
{
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::~DotationInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Equipement_Data::DotationInfos::~DotationInfos()
{
    this->Reset();
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::Reset
// Created: APE 2004-12-29
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::DotationInfos::Reset()
{
    categories_.Reset();
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::GetNodeName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::DotationInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::GetItemName
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
std::string ADN_Equipement_Data::DotationInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::GetCategories
// Created: APE 2005-01-05
// -----------------------------------------------------------------------------
ADN_Equipement_Data::T_CategoryInfos_Vector& ADN_Equipement_Data::DotationInfos::GetCategories()
{
    return categories_;
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::FindCategory
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::DotationInfos::FindCategory( const std::string& strName )
{
    IT_CategoryInfos_Vector it = std::find_if( categories_.begin(), categories_.end(), ADN_Tools::NameCmp<CategoryInfo>( strName ) );
    if( it == categories_.end() )
        return 0;

    return *it;
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::DotationInfos::ReadArchive( xml::xistream& input )
{
    std::auto_ptr< CategoryInfo > spNew;
    if( strName_.GetData() == "munition" )
        spNew.reset( new AmmoCategoryInfo( *this ) );
    else
        spNew.reset( new CategoryInfo( *this ) );
    spNew->ReadArchive( input );
    categories_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: DotationInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::DotationInfos::WriteArchive( xml::xostream& output )
{
    for( IT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
        (*it)->WriteArchive( output );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data constructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::ADN_Equipement_Data()
: ADN_Data_ABC()
, nNextCatId_ ( 1 )
, dotations_  ()
{
    for( int n = 0; n < eNbrDotationFamily; ++n )
        dotations_.AddItem( new DotationInfos( (E_DotationFamily)n ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data destructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::~ADN_Equipement_Data()
{
    dotations_.Reset();
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
    for( IT_DotationInfos_Vector it = dotations_.begin(); it != dotations_.end(); ++it )
        (*it)->Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::ReadDotation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ReadDotation( xml::xistream& input )
{
    std::string name;
    input >> xml::attribute( "name", name );
    E_DotationFamily nTypeDotation = ENT_Tr::ConvertToDotationFamily( name );
    if( nTypeDotation == -1 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid resource type '%1'" ).arg( name.c_str() ).ascii() );
    dotations_.at( nTypeDotation )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "dotations" )
            >> xml::list( "dotation", *this, &ADN_Equipement_Data::ReadDotation )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "dotations" );
    ADN_Tools::AddSchema( output, "Resources" );
    for( IT_DotationInfos_Vector it = dotations_.begin(); it != dotations_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FindEquipementCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::FindEquipementCategory( const std::string& strDotationName, const std::string& strCategoryName )
{
    IT_DotationInfos_Vector it = std::find_if( dotations_.begin(), dotations_.end(), ADN_Tools::NameCmp<DotationInfos>( strDotationName ) );
    if( it == dotations_.end() )
        return 0;

    return (*it)->FindCategory( strCategoryName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FindEquipementCategory
// Created: APE 2004-11-19
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Data::FindEquipementCategory( const std::string& strCategoryName )
{
    for ( IT_DotationInfos_Vector it = dotations_.begin(); it != dotations_.end(); ++it )
    {
        ADN_Equipement_Data::CategoryInfo* category = (*it)->FindCategory( strCategoryName );
        if ( category )
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
ADN_Equipement_Data::DotationInfos& ADN_Equipement_Data::GetDotation( E_DotationFamily nType )
{
    return * dotations_[ nType ];
}
