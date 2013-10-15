// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_WeaponFilter.h"
#include "moc_ADN_WeaponFilter.cpp"
#include "ADN_Weapons_Data_WeaponInfos.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter constructor
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
ADN_WeaponFilter::ADN_WeaponFilter()
    : pData_     ( 0 )
    , pConnector_( 0 )
    , bDirect_( false )
    , pLauncher_( nullptr )
    , pAmmo_( nullptr )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Initialize
// Created: MMC 2013-03-14
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::Initialize( ADN_Launchers_Data::LauncherInfos& launcher, bool direct )
{
    pLauncher_ = &launcher;
    pAmmo_ = nullptr;
    bDirect_ = direct;
    pData_ = direct? &launcher.bDirect_ : &launcher.bIndirect_;
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Initialize
// Created: MMC 2013-03-14
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::Initialize( ADN_Resources_Data::AmmoCategoryInfo& ammo, bool direct )
{
    pLauncher_ = nullptr;
    pAmmo_ = &ammo;
    bDirect_ = direct;
    pData_ = direct? &ammo.bDirect_ : &ammo.bIndirect_;
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Initialize
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::Initialize( ADN_Connector_ABC& connector ) const
{
    if( pData_ )
    {
        pData_->Initialize( connector );
        pConnector_ = &connector;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::SetDataPrivate
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
void ADN_WeaponFilter::SetDataPrivate( void* pData )
{
    const bool data = *static_cast< bool* >( pData );
    if( pData_ )
    {
        if( data || Check() )
            *pData_ = data;
        pData_->Initialize( *pConnector_ );
    }
}

namespace
{
    bool IsValid( const ADN_Weapons_Data_WeaponInfos& weaponInfos, bool bDirect
                , const ADN_Launchers_Data::LauncherInfos* pLauncher
                , const ADN_Resources_Data::AmmoCategoryInfo* pAmmo )
    {
        if( pLauncher && pLauncher == weaponInfos.ptrLauncher_.GetData() )
            return weaponInfos.IsValidOnChange( bDirect, true );
        else if( pAmmo && pAmmo == weaponInfos.ptrAmmunition_.GetData() )
            return weaponInfos.IsValidOnChange( bDirect, false );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_WeaponFilter::Check
// Created: MCO 2011-07-05
// -----------------------------------------------------------------------------
bool ADN_WeaponFilter::Check() const
{
    std::set< ADN_Weapons_Data_WeaponInfos* > toRemove;
    auto weapons = ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos();
    QString invalidWeapons;
    for( auto it = weapons.begin(); it != weapons.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos* pWeaponInfos = *it;
        if( !IsValid( *pWeaponInfos, bDirect_, pLauncher_, pAmmo_ ) )
        {
            toRemove.insert( pWeaponInfos );
            invalidWeapons += ( pWeaponInfos->strName_.GetData() + '\n' ).c_str();
        }
    }
    if( toRemove.empty() )
        return true;
    int result = QMessageBox::warning( 0,
                                       tr( "Warning" ),
                                       tr( "This will delete all the following associated weapon systems :" ) + "\n\n" + invalidWeapons + '\n' + tr( "Proceed anyway ?" ),
                                       QMessageBox::Ok, QMessageBox::Cancel );
    if( result == QMessageBox::Cancel )
        return false;
    for( auto it = toRemove.begin(); it != toRemove.end(); ++it )
        ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos().RemItem( *it );
    return true;
}
