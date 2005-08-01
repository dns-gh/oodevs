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

#include "ADN_pch.h"
#include "ADN_Equipement_Data.h"

#include "ADN_Tools.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
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

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: CategoryInfo::ReadArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Categorie" );

    input.ReadAttribute( "nom", strName_ );

    input.Section( "Conditionnement" );
    input.ReadField( "Nombre", rNbrInPackage_  );
    input.ReadField( "Masse" , rPackageWeight_ );
    input.ReadField( "Volume", rPackageVolume_ );
    input.EndSection(); // Conditionnement

    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_ = strName_.GetData();

    input.ReadField( "CodeEMAT6", strCodeEMAT6_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeEMAT8", strCodeEMAT8_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeLFRIL", strCodeLFRIL_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeNNO", strCodeNNO_, ADN_XmlInput_Helper::eNothing );

    input.EndSection(); // Categorie
}


// -----------------------------------------------------------------------------
// Name: CategoryInfo::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::CategoryInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Categorie" );

    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteField( "MosID", nMosId_ );

    output.Section( "Conditionnement" );
    output.WriteField( "Nombre", rNbrInPackage_ .GetData() );
    output.WriteField( "Masse" , rPackageWeight_.GetData() );
    output.WriteField( "Volume", rPackageVolume_.GetData() );
    output.EndSection(); // Conditionnement
    
    output.WriteField( "CodeEMAT6", strCodeEMAT6_.GetData() );
    output.WriteField( "CodeEMAT8", strCodeEMAT8_.GetData() );
    output.WriteField( "CodeLFRIL", strCodeLFRIL_.GetData() );
    output.WriteField( "CodeNNO", strCodeNNO_.GetData() );

    output.EndSection();
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
void ADN_Equipement_Data::AttritionInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    std::string strSection = std::string("ProtectionType") + ptrArmor_.GetData()->strName_.GetData();
    input.Section( strSection );
    input.ReadField( "Destruction", rDestroy_ );
    input.ReadField( "ReparableAvecEvacuation", rRepairWithEvac_ );
    input.ReadField( "ReparableSansEvacuation", rRepairNoEvac_ );
    rDestroy_        = rDestroy_.GetData() * 100.0;
    rRepairWithEvac_ = rRepairWithEvac_.GetData() * 100.0;
    rRepairNoEvac_   = rRepairNoEvac_.GetData() * 100.0;
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: AttritionInfos::WriteArchive
// Created: APE 2004-11-15
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AttritionInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    std::string strSection = std::string("ProtectionType") + ptrArmor_.GetData()->strName_.GetData();
    output.Section( strSection );
    output.WriteField( "Destruction", rDestroy_.GetData() / 100.0 );
    output.WriteField( "ReparableAvecEvacuation", rRepairWithEvac_.GetData() / 100.0 );
    output.WriteField( "ReparableSansEvacuation", rRepairNoEvac_.GetData() / 100.0 );
    output.EndSection();
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
, rDeployTime_      ( 0.0 )
, rLifeTime_        ( 0.0 )
, nMineNumber_      ( 0 )
{
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
    rDeployTime_ = ammoInfos.rDeployTime_.GetData();
    rLifeTime_ = ammoInfos.rLifeTime_.GetData();
    nMineNumber_ = ammoInfos.nMineNumber_.GetData();
}


// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "TirIndirect" );

    std::string strTmp;
    input.ReadAttribute( "type", strTmp );
    nIndirectType_ = ENT_Tr::ConvertToTypeMunitionTirIndirect( strTmp );
    if( nIndirectType_ == (E_TypeMunitionTirIndirect)-1 )
        input.ThrowError( MT_FormatString( "Le type de munition indirecte '%s' est invalide.", strTmp.c_str() ) );

    input.ReadField( "InterventionType", nIntervention_ );
    input.ReadField( "DispersionX", rDispersionX_ );
    input.ReadField( "DispersionY", rDispersionY_ );

    switch( nIndirectType_.GetData() )
    {
        case eTypeMunitionTirIndirect_Explosif:
        case eTypeMunitionTirIndirect_Aced:
        case eTypeMunitionTirIndirect_Grenade:
            input.ReadField( "CoefficientNeutralisation", rNeutralizationRatio_ );
            break;

        case eTypeMunitionTirIndirect_Fumigene:
        case eTypeMunitionTirIndirect_Eclairant:
            input.ReadTimeField( "DureeDeploiement", rDeployTime_ );
            input.ReadTimeField( "DureeVie", rLifeTime_ );
            break;

        case eTypeMunitionTirIndirect_Mine:
            input.ReadField( "NombreMines", nMineNumber_ );
            break;
    }

    input.EndSection(); // TirIndirect
}


// -----------------------------------------------------------------------------
// Name: IndirectAmmoInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::IndirectAmmoInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "TirIndirect" );
    output.WriteAttribute( "type", ENT_Tr::ConvertFromTypeMunitionTirIndirect( nIndirectType_.GetData() ) );

    output.WriteField( "InterventionType", nIntervention_.GetData() );
    output.WriteField( "DispersionX", rDispersionX_.GetData() );
    output.WriteField( "DispersionY", rDispersionY_.GetData() );

    switch( nIndirectType_.GetData() )
    {
    case eTypeMunitionTirIndirect_Explosif:
    case eTypeMunitionTirIndirect_Aced:
    case eTypeMunitionTirIndirect_Grenade:
        output.WriteField( "CoefficientNeutralisation", rNeutralizationRatio_.GetData() );
        break;

    case eTypeMunitionTirIndirect_Fumigene:
    case eTypeMunitionTirIndirect_Eclairant:
        output.WriteField( "DureeDeploiement", ADN_Tools::SecondToString( rDeployTime_.GetData() ) );
        output.WriteField( "DureeVie", ADN_Tools::SecondToString( rLifeTime_.GetData() ) );
        break;

    case eTypeMunitionTirIndirect_Mine:
        output.WriteField( "NombreMines", nMineNumber_.GetData() );
        break;
    }

    output.EndSection();
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

    for( uint n = 0; n < attritions_.size(); ++n )
        pCopy->attritions_[n]->CopyFrom( * attritions_[n] );
    pCopy->indirectAmmoInfos_.CopyFrom( indirectAmmoInfos_ );

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Categorie" );
    input.ReadAttribute( "nom", strName_ );
    if( input.Section( "TrancheD", ADN_XmlInput_Helper::eNothing ) )
    {
        bTrancheD_ = true;
        input.EndSection(); // TrancheD
    }

    input.ReadField( "Type", nType_, ADN_Tr::ConvertToMunitionType, ADN_XmlInput_Helper::eThrow );

    input.Section( "Conditionnement" );
    input.ReadField( "Nombre", rNbrInPackage_  );
    input.ReadField( "Masse" , rPackageWeight_ );
    input.ReadField( "Volume", rPackageVolume_ );
    input.EndSection(); // Conditionnement

    strCodeEMAT6_ = strName_.GetData();
    strCodeEMAT8_ = strName_.GetData();
    strCodeLFRIL_ = strName_.GetData();
    strCodeNNO_ = strName_.GetData();

    input.ReadField( "CodeEMAT6", strCodeEMAT6_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeEMAT8", strCodeEMAT8_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeLFRIL", strCodeLFRIL_, ADN_XmlInput_Helper::eNothing );
    input.ReadField( "CodeNNO", strCodeNNO_, ADN_XmlInput_Helper::eNothing );

    if( input.Section( "Attritions", ADN_XmlInput_Helper::eNothing ) )
    {
        bDirect_ = true;

        ADN_Categories_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
        for( ADN_Categories_Data::T_ArmorInfos_Vector::iterator it = armorInfos.begin(); it != armorInfos.end(); ++it )
        {
            IT_AttritionInfos_Vector itAttrition = std::find_if( attritions_.begin(), attritions_.end(),AttritionInfos::CmpRef(*it));
            assert( itAttrition != attritions_.end() );
            (*itAttrition)->ReadArchive( input );
        }
            
        input.EndSection(); // Attritions
    }

    if( input.Section( "TirIndirect", ADN_XmlInput_Helper::eNothing ) )
    {
        input.EndSection(); // TirIndirect
        bIndirect_ = true;
        indirectAmmoInfos_.ReadArchive( input );
    }

    input.EndSection(); // Categorie
}


// -----------------------------------------------------------------------------
// Name: AmmoCategoryInfo::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::AmmoCategoryInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Categorie" );
    output.WriteAttribute( "nom", strName_.GetData() );
    if( bTrancheD_.GetData() )
    {
        output.Section( "TrancheD" );
        output.EndSection(); // TrancheD
    }
    output.WriteField( "MosID", nMosId_ );
    output.WriteField( "Type", ADN_Tr::ConvertFromMunitionType( nType_.GetData() ) );

    output.Section( "Conditionnement" );
    output.WriteField( "Nombre", rNbrInPackage_ .GetData() );
    output.WriteField( "Masse" , rPackageWeight_.GetData() );
    output.WriteField( "Volume", rPackageVolume_.GetData() );
    output.EndSection(); // Conditionnement

    output.WriteField( "CodeEMAT6", strCodeEMAT6_.GetData() );
    output.WriteField( "CodeEMAT8", strCodeEMAT8_.GetData() );
    output.WriteField( "CodeLFRIL", strCodeLFRIL_.GetData() );
    output.WriteField( "CodeNNO", strCodeNNO_.GetData() );

    if( bDirect_.GetData() == true )
    {
        output.Section( "Attritions" ); 

        ADN_Categories_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
        for( ADN_Categories_Data::T_ArmorInfos_Vector::iterator it = armorInfos.begin(); it != armorInfos.end(); ++it )
        {
            IT_AttritionInfos_Vector itAttrition = std::find_if( attritions_.begin(), attritions_.end(),AttritionInfos::CmpRef(*it));
            assert( itAttrition != attritions_.end() );
            (*itAttrition)->WriteArchive( output );
        }

        output.EndSection(); // Attritions
    }

    if( bIndirect_.GetData() == true )
        indirectAmmoInfos_.WriteArchive( output );

    output.EndSection(); // Categorie
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::DotationInfos
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::DotationInfos::DotationInfos( E_FamilleDotation nType )
: ADN_Ref_ABC            ()
, ADN_DataTreeNode_ABC   ()
, nType_                 ( nType )
, strName_               ( ENT_Tr::ConvertFromFamilleDotation( nType ) )
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
void ADN_Equipement_Data::DotationInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
//    input.Section( "Dotation" );
//    input.ReadAttribute( "nom", strName_ );

    input.BeginList( "Categories" );
    while( input.NextListElement() )
    {
        CategoryInfo* pNew = 0;
        if( strName_.GetData() == "munition" )
            pNew = new AmmoCategoryInfo( *this );
        else
            pNew = new CategoryInfo( *this );

        std::auto_ptr< CategoryInfo > spNew( pNew );

        spNew->ReadArchive( input );
        categories_.AddItem( spNew.release() );
    }
    input.EndList(); // Categories

//    input.EndSection(); // Dotation
}


// -----------------------------------------------------------------------------
// Name: DotationInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::DotationInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Dotation" );
    output.WriteAttribute( "nom", strName_.GetData() );

    output.BeginList( "Categories", categories_.size() );
    for( IT_CategoryInfos_Vector it = categories_.begin(); it != categories_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // Categories

    output.EndSection(); // Dotation 
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
    for( int n = 0; n < eNbrFamilleDotation; ++n )
        dotations_.AddItem( new DotationInfos( (E_FamilleDotation)n ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data destructor
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
ADN_Equipement_Data::~ADN_Equipement_Data()
{
    nNextCatId_ = 1;
    dotations_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::FilesNeeded
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::FilesNeeded(T_StringList& files ) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szEquipements_.GetData());
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::Reset
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::Reset()
{
    for( IT_DotationInfos_Vector it = dotations_.begin(); it != dotations_.end(); ++it )
        (*it)->Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Dotations" );
    while( input.NextListElement() )
    {
        input.Section( "Dotation" );

        std::string strName;
        input.ReadAttribute( "nom", strName );

        E_FamilleDotation nTypeDotation = ENT_Tr::ConvertToFamilleDotation( strName );
        if( nTypeDotation == -1 )
        {
            assert( 0 );
            return;
        }

        dotations_[ nTypeDotation ]->ReadArchive( input );
        input.EndSection(); // Dotation
    }

    input.EndList(); // Dotations
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Equipement_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Dotations", dotations_.size() );
    for( IT_DotationInfos_Vector it = dotations_.begin(); it != dotations_.end(); ++it )
        (*it)->WriteArchive( output );

    output.EndList(); // Dotations
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
ADN_Equipement_Data::DotationInfos& ADN_Equipement_Data::GetDotation( E_FamilleDotation nType )
{
    return * dotations_[ nType ];
}

