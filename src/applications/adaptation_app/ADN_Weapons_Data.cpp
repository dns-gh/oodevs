// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_Data.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 10:03 $
// $Revision: 13 $
// $Workfile: ADN_Weapons_Data.cpp $
//
// *****************************************************************************
#include "ADN_pch.h"
#include "ADN_Weapons_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"


// -----------------------------------------------------------------------------
// Name: PhInfos::PhInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::PhInfos::PhInfos()
: ADN_DataTreeNode_ABC()
, nDistance_(0)
, rPerc_(0.0)
{
    nDistance_.SetParentNode( *this );
    rPerc_.SetParentNode( *this );
    rPerc_.SetDataName( "le pourcentage de chance de toucher à" );
}


// -----------------------------------------------------------------------------
// Name: PhInfos::GetNodeName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhInfos::GetNodeName()
{
    return std::string( "la distance de tir " ) + itostring( nDistance_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: PhInfos::GetItemName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhInfos::GetItemName()
{
    return std::string( "à " ) + itostring( nDistance_.GetData() ) + " mètres";
}


// -----------------------------------------------------------------------------
// Name: PhInfos::CreateCopy
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapons_Data::PhInfos* ADN_Weapons_Data::PhInfos::CreateCopy()
{
    PhInfos* pCopy = new PhInfos();
    pCopy->nDistance_ = nDistance_.GetData();
    pCopy->rPerc_ = rPerc_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: PhInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "PH" );
    input.ReadAttribute( "dist", nDistance_ );
    double rPerc;
    input.Read( rPerc );
    rPerc_ = rPerc * 100.0;
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: PhInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "PH" );
    output.WriteAttribute( "dist", nDistance_.GetData() );
    output << rPerc_.GetData() / 100.0;
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::PhSizeInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::PhSizeInfos::PhSizeInfos( ADN_Categories_Data::SizeInfos* ptr )
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, ptrSize_              ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), ptr )
, vPhs_                 ( false )
{
    this->BindExistenceTo(&ptrSize_);
    vPhs_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::~PhSizeInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::PhSizeInfos::~PhSizeInfos()
{
    vPhs_.Reset();
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::GetNodeName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhSizeInfos::GetNodeName()
{
    std::string strResult( "sur une cible de taille " );
    return strResult + ptrSize_.GetData()->GetData();
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::GetItemName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhSizeInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhSizeInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    std::string strSection = std::string("VolumeCible") + ptrSize_.GetData()->GetData();
    input.BeginList( strSection );
    while( input.NextListElement() )
    {
        std::auto_ptr<PhInfos> spNew( new PhInfos() );
        spNew->ReadArchive( input );
        vPhs_.AddItem( spNew.release() );
    }
    input.EndList();
}


// -----------------------------------------------------------------------------
// Name: PhSizeInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhSizeInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    std::string strSection = std::string("VolumeCible") + ptrSize_.GetData()->GetData();
    output.BeginList( strSection, vPhs_.size() );
    for( IT_PhInfosVector it = vPhs_.begin(); it != vPhs_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList();
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::WeaponInfos::WeaponInfos()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, ptrLauncher_      ( ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos(), 0 )
, ptrAmmunition_    ( (ADN_Equipement_Data::T_AmmoCategoryInfo_Vector&)ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eFamilleDotation_Munition ).categories_, 0, "" )
, nRoundsPerBurst_  ( 0 )
, burstDuration_    ( "0s" )
, nRoundsPerReload_ ( 0 )
, reloadDuration_   ( "0s" )
, bDirect_      ( false )
, phs_          ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos() )
, bIndirect_    ( false )
, rAverageSpeed_( 0 )
, rMinRange_    ( 0 )
, rMaxRange_    ( 0 )
{
    BindExistenceTo( &ptrLauncher_ );
    BindExistenceTo( &ptrAmmunition_ );
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::GetNodeName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::WeaponInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::GetItemName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::WeaponInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::CreateCopy
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapons_Data::WeaponInfos* ADN_Weapons_Data::WeaponInfos::CreateCopy()
{
    WeaponInfos* pCopy = new WeaponInfos();
    pCopy->strName_ = strName_.GetData();
    pCopy->ptrLauncher_ = ptrLauncher_.GetData();
    pCopy->ptrAmmunition_ = ptrAmmunition_.GetData();
    pCopy->nRoundsPerBurst_ = nRoundsPerBurst_.GetData();
    pCopy->burstDuration_ = burstDuration_.GetData();
    pCopy->nRoundsPerReload_ = nRoundsPerReload_.GetData();
    pCopy->reloadDuration_ = reloadDuration_.GetData();
    pCopy->bDirect_ = bDirect_.GetData();

    for( uint n = 0; n < phs_.size(); ++n )
    {
        PhSizeInfos& phSizeInfo = * phs_[n];
        PhSizeInfos& phSizeInfoCopy = * pCopy->phs_[n];
        for( IT_PhInfosVector it = phSizeInfo.vPhs_.begin(); it != phSizeInfo.vPhs_.end(); ++it )
        {
            phSizeInfoCopy.vPhs_.AddItem( (*it)->CreateCopy() );
        }
    }

    pCopy->bIndirect_ = bIndirect_.GetData();
    pCopy->rAverageSpeed_ = rAverageSpeed_.GetData();
    pCopy->rMinRange_ = rMinRange_.GetData();
    pCopy->rMaxRange_ = rMaxRange_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WeaponInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Armement" );

    std::string strLauncher;
    std::string strAmmunition;
    input.ReadAttribute( "lanceur", strLauncher );
    input.ReadAttribute( "munition", strAmmunition );

    ADN_Launchers_Data::LauncherInfos* pLauncher = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().FindLauncher( strLauncher );
    if( !pLauncher )
        input.ThrowError( MT_FormatString( "Armement lanceur '%s' / dotation '%s' : type de lanceur invalide", strLauncher.c_str(), strAmmunition.c_str() ) );

    ptrLauncher_ = pLauncher;

    ADN_Equipement_Data::CategoryInfo* pAmmo = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( "munition", strAmmunition );
    if( !pAmmo )
        input.ThrowError( MT_FormatString( "Armement lanceur '%s' / dotation '%s' : type de dotation invalide", strLauncher.c_str(), strAmmunition.c_str() ) );

    assert( pAmmo != 0 );
    ptrAmmunition_ = (ADN_Equipement_Data::AmmoCategoryInfo*)pAmmo;

    strName_ = strLauncher + " & " + strAmmunition;

    input.ReadField( "NbrMunitionParRafale", nRoundsPerBurst_ );
    input.ReadField( "DureeRafale", burstDuration_ );
    input.ReadField( "NbrMunitionsParRechargement", nRoundsPerReload_ );
    input.ReadField( "DureeRechargement", reloadDuration_ );

    if( input.Section( "Direct", ADN_XmlInput_Helper::eNothing ) )
    {
        bDirect_ = true;
        input.Section( "PHs" );
        ADN_Categories_Data::T_SizeInfos_Vector& sizeInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos();
        for( ADN_Categories_Data::IT_SizeInfos_Vector it = sizeInfos.begin(); it != sizeInfos.end(); ++it )
        {
            IT_PhSizeInfosVector itPhSizeInfo = std::find_if( phs_.begin(), phs_.end(), PhSizeInfos::CmpRef(*it));
            assert( itPhSizeInfo != phs_.end() );
            (*itPhSizeInfo)->ReadArchive( input );
        }

        input.EndSection(); // PHs
        input.EndSection(); // Direct
    }

    if( input.Section( "Indirect", ADN_XmlInput_Helper::eNothing ) )
    {
        bIndirect_ = true;
        input.ReadField( "VitesseMoyenne", rAverageSpeed_ );
        input.ReadField( "PorteeMin", rMinRange_ );
        input.ReadField( "PorteeMax", rMaxRange_ );
        input.EndSection(); // Indirect
    }

    input.EndSection(); // Armement
}


// -----------------------------------------------------------------------------
// Name: WeaponInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WeaponInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Armement" );

    output.WriteAttribute( "lanceur", ptrLauncher_.GetData()->strName_.GetData() );
    output.WriteAttribute( "munition", ptrAmmunition_.GetData()->strName_.GetData() );

    //$$$$$

    output.WriteField( "NbrMunitionParRafale", nRoundsPerBurst_.GetData() );
    output.WriteField( "DureeRafale", burstDuration_.GetData() );
    output.WriteField( "NbrMunitionsParRechargement", nRoundsPerReload_.GetData() );
    output.WriteField( "DureeRechargement", reloadDuration_.GetData() );

    if( bDirect_.GetData() )
    {
        output.Section( "Direct" );
        output.Section( "PHs" );
        ADN_Categories_Data::T_SizeInfos_Vector& sizeInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos();
        for( ADN_Categories_Data::IT_SizeInfos_Vector it = sizeInfos.begin(); it != sizeInfos.end(); ++it )
        {
            IT_PhSizeInfosVector itPhSizeInfo = std::find_if( phs_.begin(), phs_.end(), PhSizeInfos::CmpRef(*it));
            assert( itPhSizeInfo != phs_.end() );
            (*itPhSizeInfo)->WriteArchive( output );
        }

        output.EndSection(); // PHs
        output.EndSection(); // Direct
    }

    if( bIndirect_.GetData() )
    {
        output.Section( "Indirect" );
        output.WriteField( "VitesseMoyenne", rAverageSpeed_.GetData() );

        if( rMaxRange_.GetData() < rMinRange_.GetData() )
            throw ADN_DataException( tr( "Data errror" ).ascii(), tr( "In the indirect fire parameters of weapon %1, the max range is inferior to min range." ).arg( strName_.GetData().c_str() ).ascii() );

        output.WriteField( "PorteeMin", rMinRange_.GetData() );
        output.WriteField( "PorteeMax", rMaxRange_.GetData() );

        output.EndSection(); // Indirect
    }

    output.EndSection(); // Armement
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data constructor
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::ADN_Weapons_Data()
: ADN_Data_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data destructor
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::~ADN_Weapons_Data()
{
    this->Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::FilesNeeded
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szWeapons_.GetData());
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::Reset
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::Reset()
{
    weapons_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Armements" );
    while( input.NextListElement() )
    {
        std::auto_ptr<WeaponInfos> spNew( new WeaponInfos() );
        spNew->ReadArchive( input );
        weapons_.AddItem( spNew.release() );
    }
    input.EndList(); // Armements
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Armements", weapons_.size() );
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponThatUse
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::GetWeaponThatUse( ADN_Launchers_Data::LauncherInfos& launcher )
{
    std::string strResult;
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        WeaponInfos* pWeapon = *it;
        if( pWeapon->ptrLauncher_ == &launcher )
        {
            if( strResult != "" )
                strResult += "<br>";
            strResult += "<nobr>" + pWeapon->strName_.GetData() + "</nobr>";
        }
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponThatUse
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::GetWeaponThatUse( ADN_Equipement_Data::AmmoCategoryInfo& ammunition )
{
    std::string strResult;
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        WeaponInfos* pWeapon = *it;
        if( pWeapon->ptrAmmunition_ == &ammunition )
        {
            if( strResult != "" )
                strResult += "<br>";
            strResult += "<nobr>" + pWeapon->strName_.GetData() + "</nobr>";
        }
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::UpdateNames
// Created: SBO 2005-09-15
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::UpdateNames()
{
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        WeaponInfos* pWeapon = *it;
        pWeapon->strName_ = pWeapon->ptrLauncher_.GetData()->strName_.GetData() 
                          + " & " 
                          + pWeapon->ptrAmmunition_.GetData()->strName_.GetData();
    }
}
