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
#include "adaptation_app_pch.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tr.h"
#include "MT_Tools/MT_FormatString.h"

// -----------------------------------------------------------------------------
// Name: PhInfos::PhInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::PhInfos::PhInfos()
: ADN_DataTreeNode_ABC()
, nDistance_(0)
, rPerc_(0.0)
, nModifiedDistance_( 0 )
, rModifiedPerc_( 0.0 )
, distModifier_( 1.0 )
, phModifier_( 1.0 )
{
    nDistance_.SetParentNode( *this );
    nModifiedDistance_.SetParentNode( *this );
    rPerc_.SetParentNode( *this );
    rPerc_.SetDataName( "le pourcentage de chance de toucher à" );
    rModifiedPerc_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: PhInfos::GetNodeName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhInfos::GetNodeName()
{
    return MT_FormatString( "la distance de tir %d", nDistance_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: PhInfos::GetItemName
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
std::string ADN_Weapons_Data::PhInfos::GetItemName()
{
    return MT_FormatString( "à %d mètres", nDistance_.GetData() );
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
    pCopy->nModifiedDistance_ = nModifiedDistance_.GetData();
    pCopy->rModifiedPerc_ = rModifiedPerc_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::SetPhModifiers
// Created: JSR 2010-04-27
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::SetPhModifiers( double distModifier, double phModifier )
{
    distModifier_ = distModifier;
    phModifier_ = phModifier;
    ApplyPhModifiers();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::ApplyPhModifiers
// Created: JSR 2010-04-27
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::ApplyPhModifiers()
{
    nModifiedDistance_ = ( int ) ( nDistance_.GetData() * distModifier_ );
    rModifiedPerc_ = rPerc_.GetData() * phModifier_;
}

// -----------------------------------------------------------------------------
// Name: PhInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::ReadArchive( xml::xistream& input )
{
    double rPerc;
    input >> xml::attribute( "distance", nDistance_ )
          >> xml::attribute( "percentage", rPerc );
    rPerc_ = rPerc * 100.;
}

// -----------------------------------------------------------------------------
// Name: PhInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "hit-probability" )
            << xml::attribute( "distance", nDistance_ )
            << xml::attribute( "percentage", rPerc_.GetData() / 100.0 )
           << xml::end;
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
// Name: ADN_Weapons_Data::PhSizeInfos::ReadHp
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhSizeInfos::ReadHp( xml::xistream& input )
{
    std::auto_ptr<PhInfos> spNew( new PhInfos() );
    spNew->ReadArchive( input );
    vPhs_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: PhSizeInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhSizeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "hit-probability", *this, &ADN_Weapons_Data::PhSizeInfos::ReadHp );
}

// -----------------------------------------------------------------------------
// Name: PhSizeInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::PhSizeInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "hit-probabilities" )
            << xml::attribute( "target", *ptrSize_.GetData() );
    for( IT_PhInfosVector it = vPhs_.begin(); it != vPhs_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::WeaponInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::WeaponInfos::WeaponInfos()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, ptrLauncher_      ( ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos(), 0 )
, ptrAmmunition_    ( (ADN_Equipement_Data::T_AmmoCategoryInfo_Vector&)ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0, "" )
, nRoundsPerBurst_  ( 0 )
, burstDuration_    ( "0s" )
, nRoundsPerReload_ ( 0 )
, reloadDuration_   ( "0s" )
, bDirect_        ( false )
, phs_            ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos() )
, bSimulation_    ( false )
, nFirePosture_   ( eUnitPosture_PostureArret )
, nTargetPosture_ ( eUnitPosture_PostureArret )
, nExperience_    ( eUnitExperience_Veteran )
, nTiredness_     ( eUnitTiredness_Normal )
, nStress_        ( eUnitStress_Calm )
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
            phSizeInfoCopy.vPhs_.AddItem( (*it)->CreateCopy() );
    }

    pCopy->bSimulation_ = bSimulation_.GetData();
    pCopy->nFirePosture_ = nFirePosture_.GetData();
    pCopy->nTargetPosture_ = nTargetPosture_.GetData();
    pCopy->nExperience_ = nExperience_.GetData();
    pCopy->nTiredness_ = nTiredness_.GetData();
    pCopy->nStress_ = nStress_.GetData();

    pCopy->bIndirect_ = bIndirect_.GetData();
    pCopy->rAverageSpeed_ = rAverageSpeed_.GetData();
    pCopy->rMinRange_ = rMinRange_.GetData();
    pCopy->rMaxRange_ = rMaxRange_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::WeaponInfos::ReadTargetSize
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WeaponInfos::ReadTargetSize( xml::xistream& input )
{
    bDirect_ = true;
    std::string target = input.attribute< std::string >( "target" );
    IT_PhSizeInfosVector itPhSizeInfo = std::find_if( phs_.begin(), phs_.end(), PhSizeInfos::Cmp( target ));
    if( itPhSizeInfo != phs_.end() )
        (*itPhSizeInfo)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WeaponInfos::ReadArchive( xml::xistream& input )
{
    std::string strLauncher, strAmmunition;
    input >> xml::attribute( "launcher", strLauncher )
          >> xml::attribute( "munition", strAmmunition );

    ADN_Launchers_Data::LauncherInfos* pLauncher = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().FindLauncher( strLauncher );
    if( !pLauncher )
        throw ADN_DataException( tools::translate( "Weapons_Data", "Invalid data" ).ascii(), tools::translate( "Weapons_Data", "Weapon systems '%1'/'%2' - Invalid launcher type" ).arg( strLauncher.c_str(), strAmmunition.c_str() ).ascii() );
    ptrLauncher_ = pLauncher;

    ADN_Equipement_Data::CategoryInfo* pAmmo = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( "munition", strAmmunition );
    if( !pAmmo )
        throw ADN_DataException( tools::translate( "Weapons_Data", "Invalid data" ).ascii(), tools::translate( "Weapons_Data", "Weapon systems '%1'/'%2' - Invalid ammunition type" ).arg( strLauncher.c_str(), strAmmunition.c_str() ).ascii() );
    ptrAmmunition_ = (ADN_Equipement_Data::AmmoCategoryInfo*)pAmmo;

    strName_ = strLauncher + " & " + strAmmunition;
    input >> xml::start( "burst" )
            >> xml::attribute( "munition", nRoundsPerBurst_ )
            >> xml::attribute( "duration", burstDuration_ )
          >> xml::end
          >> xml::start( "reloading" )
            >> xml::attribute( "munition", nRoundsPerReload_ )
            >> xml::attribute( "duration", reloadDuration_ )
          >> xml::end;
    input >> xml::optional
          >> xml::start( "direct-fire" )
            >> xml::list( "hit-probabilities", *this, &ADN_Weapons_Data::WeaponInfos::ReadTargetSize )
          >> xml::end
          >> xml::optional
          >> xml::start( "indirect-fire" )
            >> xml::attribute( "average-speed", rAverageSpeed_ )
            >> xml::attribute( "min-range",     rMinRange_ )
            >> xml::attribute( "max-range",     rMaxRange_ )
          >> xml::end;

    bIndirect_ = rMaxRange_ != .0;
}

// -----------------------------------------------------------------------------
// Name: WeaponInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WeaponInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "weapon-system" )
            << xml::attribute( "launcher", ptrLauncher_  .GetData()->strName_ )
            << xml::attribute( "munition", ptrAmmunition_.GetData()->strName_ )
           << xml::start( "burst" )
            << xml::attribute( "munition", nRoundsPerBurst_ )
            << xml::attribute( "duration", burstDuration_ )
           << xml::end
           << xml::start( "reloading" )
            << xml::attribute( "munition", nRoundsPerReload_ )
            << xml::attribute( "duration", reloadDuration_ )
           << xml::end;

    if( bDirect_.GetData() )
    {
        output << xml::start( "direct-fire" );
        for( IT_PhSizeInfosVector it = phs_.begin(); it != phs_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end;
    }

    if( bIndirect_.GetData() )
    {
        if( rMaxRange_.GetData() < rMinRange_.GetData() )
            throw ADN_DataException( tools::translate( "Weapons_Data", "Invalid data" ).ascii(), tools::translate( "Weapons_Data", "Weapon %1 - Indirect fire - max range < min range" ).arg( strName_.GetData().c_str() ).ascii() );

        output << xml::start( "indirect-fire" )
                << xml::attribute( "average-speed", rAverageSpeed_ )
                << xml::attribute( "min-range",     rMinRange_ )
                << xml::attribute( "max-range",     rMaxRange_ )
               << xml::end;
    }

    output << xml::end;
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
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szWeapons_.GetData());
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
void ADN_Weapons_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "weapons" )
            >> xml::list( "weapon-system", *this, &ADN_Weapons_Data::ReadWeapon )
           >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::ReadWeapon
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::ReadWeapon( xml::xistream& input )
{
    std::auto_ptr<WeaponInfos> spNew( new WeaponInfos() );
    spNew->ReadArchive( input );
    weapons_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "weapons" );
    ADN_Tools::AddSchema( output, "WeaponSystems" );
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponThatUse
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
QStringList ADN_Weapons_Data::GetWeaponThatUse( ADN_Launchers_Data::LauncherInfos& launcher )
{
    QStringList result;
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        WeaponInfos* pWeapon = *it;
        if( pWeapon->ptrLauncher_ == &launcher )
            result << pWeapon->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponThatUse
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
QStringList ADN_Weapons_Data::GetWeaponThatUse( ADN_Equipement_Data::AmmoCategoryInfo& ammunition )
{
    QStringList result;
    for( IT_WeaponInfosVector it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        WeaponInfos* pWeapon = *it;
        if( pWeapon->ptrAmmunition_ == &ammunition )
            result << pWeapon->strName_.GetData().c_str();
    }
    return result;
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
