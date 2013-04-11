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
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data constructor
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data::ADN_Weapons_Data()
    : ADN_Data_ABC( eWeapons )
{
    // NOTHING
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
void ADN_Weapons_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szWeapons_ );
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
    std::auto_ptr< ADN_Weapons_Data_WeaponInfos > spNew( new ADN_Weapons_Data_WeaponInfos() );
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
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
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
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos* pWeapon = *it;
        if( pWeapon->ptrLauncher_ == &launcher )
            result << pWeapon->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponThatUse
// Created: SBO 2005-09-06
// -----------------------------------------------------------------------------
QStringList ADN_Weapons_Data::GetWeaponThatUse( ADN_Resources_Data::AmmoCategoryInfo& ammunition )
{
    QStringList result;
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos* pWeapon = *it;
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
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos* pWeapon = *it;
        if( !pWeapon->ptrLauncher_.GetData() || !pWeapon->ptrAmmunition_.GetData() )
            continue;
        pWeapon->strName_ = pWeapon->ptrLauncher_.GetData()->strName_.GetData()
                          + " & "
                          + pWeapon->ptrAmmunition_.GetData()->strName_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponInfos
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
ADN_Type_Vector_ABC< ADN_Weapons_Data_WeaponInfos >& ADN_Weapons_Data::GetWeaponInfos()
{
    return weapons_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::FindWeapon
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
ADN_Weapons_Data_WeaponInfos* ADN_Weapons_Data::FindWeapon( const std::string& strLauncher, const std::string& strAmmunition )
{
    auto it = std::find_if( weapons_.begin(), weapons_.end(), ADN_Weapons_Data_WeaponInfos::Cmp( strLauncher, strAmmunition ) );
    if( it == weapons_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetIndex
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
int ADN_Weapons_Data::GetIndex( ADN_Weapons_Data_WeaponInfos& weapon )
{
    auto it = std::find( weapons_.begin(), weapons_.end(), &weapon );
    assert( it != weapons_.end() );
    return static_cast< int >( std::distance( weapons_.begin(), it ) );
}
