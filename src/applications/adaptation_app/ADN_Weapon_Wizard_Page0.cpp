// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapon_Wizard_Page0.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 15:18 $
// $Revision: 6 $
// $Workfile: ADN_Weapon_Wizard_Page0.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapon_Wizard_Page0.h"

#include "clients_kernel/Tools.h"
#include <QtGui/qcombobox.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3buttongroup.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qmessagebox.h>
#include <Qt3Support/q3wizard.h>

#include "ADN_Workspace.h"
#include "ADN_UserData_ListViewItem.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapon_Wizard_Page0 constructor
// Created: APE 2005-01-11
// -----------------------------------------------------------------------------
ADN_Weapon_Wizard_Page0::ADN_Weapon_Wizard_Page0( Q3Wizard* pParent, const char* szName )
: ADN_Wizard_FirstPage_ABC<ADN_Weapons_Data::WeaponInfos>( pParent, szName )
{
    pRightSide_->setSpacing( 10 );
    pRightSide_->setFixedHeight( 200 );

    Q3HBox* pParamBox = new Q3HBox( pRightSide_ );
    pComboLauncher_ = new QComboBox( pParamBox );
    pComboAmmo_ = new QComboBox( pParamBox );

    ADN_Launchers_Data::T_LauncherInfos_Vector& vLaunchers = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos();
    for( ADN_Launchers_Data::IT_LauncherInfos_Vector itLauncher = vLaunchers.begin(); itLauncher != vLaunchers.end(); ++itLauncher )
        pComboLauncher_->insertItem( (*itLauncher)->strName_.GetData().c_str() );
    pComboLauncher_->model()->sort( 0 );

    ADN_Equipement_Data::ResourceInfos& ammoDotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition );
    ADN_Equipement_Data::T_CategoryInfos_Vector& vAmmo = ammoDotation.GetCategories();
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector itAmmo = vAmmo.begin(); itAmmo != vAmmo.end(); ++itAmmo )
        pComboAmmo_->insertItem( (*itAmmo)->strName_.GetData().c_str() );
    pComboAmmo_->model()->sort( 0 );

    Q3VBox* pBox = new Q3VBox( pRightSide_ );
    pBox->setSpacing( 5 );

    Q3ButtonGroup* pButtonGroup = new Q3ButtonGroup( 2, Qt::Vertical, pBox );
    pButtonGroup->setMargin( 0 );
    pButtonGroup->setLineWidth( 0 );
                        new QRadioButton( tools::translate( "ADN_Weapon_Wizard_Page0", "Create new" ), pButtonGroup );
    pCopyRadioButton_ = new QRadioButton( tools::translate( "ADN_Weapon_Wizard_Page0", "Create a copy of:" ),    pButtonGroup );

    pWeaponList_ = new Q3ListView( pBox );
    pWeaponList_->addColumn( tools::translate( "ADN_Weapon_Wizard_Page0", "Weapon systems" ) );
    pWeaponList_->setColumnWidth( 0, 350 );
    pWeaponList_->setMultiSelection( false );
    pWeaponList_->setFixedWidth( 350 );

    // fill the list
    ADN_Weapons_Data::T_WeaponInfosVector& vAll = ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos();
    for( ADN_Weapons_Data::T_WeaponInfosVector::iterator it = vAll.begin(); it != vAll.end(); ++it )
    {
        new ADN_UserData_ListViewItem< ADN_Weapons_Data::WeaponInfos >( pWeaponList_, (*it)->strName_.GetData(), (*it) );
    }

    if( pWeaponList_->firstChild() != 0 )
        pWeaponList_->setSelected( pWeaponList_->firstChild(), true );

    connect( pCopyRadioButton_, SIGNAL( toggled( bool ) ), pWeaponList_, SLOT( setEnabled( bool ) ) );
    pButtonGroup->setButton( 0 );
    pWeaponList_->setEnabled( false );

    pParent->addPage( this, tools::translate( "ADN_Weapon_Wizard_Page0", "New weapon creation" ) );
    pParent->setBackEnabled( this, false );
    pParent->setNextEnabled( this, true );
    pParent->setHelpEnabled( this, false );
    pParent->setFinishEnabled( this, false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapon_Wizard_Page0 destructor
// Created: APE 2005-01-11
// -----------------------------------------------------------------------------
ADN_Weapon_Wizard_Page0::~ADN_Weapon_Wizard_Page0()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapon_Wizard_Page0::CreateObject
// Created: APE 2005-01-11
// -----------------------------------------------------------------------------
ADN_Weapons_Data::WeaponInfos* ADN_Weapon_Wizard_Page0::CreateObject()
{
    std::string strLauncher = pComboLauncher_->currentText().toUtf8().constData();
    std::string strAmmo = pComboAmmo_->currentText().toUtf8().constData();

    ADN_Weapons_Data::WeaponInfos* pExistingWeapon = ADN_Workspace::GetWorkspace().GetWeapons().GetData().FindWeapon( strLauncher, strAmmo );
    if( pExistingWeapon != 0 )
    {
        QMessageBox::warning( this,
            tools::translate( "ADN_Weapon_Wizard_Page0", "Warning" ),
            tools::translate( "ADN_Weapon_Wizard_Page0", "A weapon using the same launcher and ammunition already exists. Please modify your choices." ),
            QMessageBox::Ok, Qt::NoButton );
        return 0;
    }

    ADN_Launchers_Data::LauncherInfos* pLauncher = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().FindLauncher( strLauncher );
    assert( pLauncher != 0 );

    ADN_Equipement_Data::ResourceInfos& ammoDotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition );
    ADN_Equipement_Data::CategoryInfo* pAmmo = ammoDotation.FindCategory( strAmmo );
    assert( pAmmo != 0 );

    ADN_Equipement_Data::AmmoCategoryInfo& ammo = dynamic_cast< ADN_Equipement_Data::AmmoCategoryInfo& >( *pAmmo );
    if( ammo.bDirect_.GetData() != pLauncher->bDirect_.GetData() &&
        ammo.bIndirect_.GetData() != pLauncher->bIndirect_.GetData() )
    {
        QMessageBox::warning( this,
            tools::translate( "ADN_Weapon_Wizard_Page0", "Warning" ),
            tools::translate( "ADN_Weapon_Wizard_Page0", "The weapon and ammunition are not compatible (direct/indirect). Please modify your choices." ),
            QMessageBox::Ok, QMessageBox::NoButton );
        return 0;
    }

    ADN_Weapons_Data::WeaponInfos* pResult = 0;

    if( pCopyRadioButton_->isOn() )
    {
        Q3ListViewItem* pSelectedItem = pWeaponList_->selectedItem();
        ADN_Weapons_Data::WeaponInfos* pWeapon = static_cast< ADN_UserData_ListViewItem< ADN_Weapons_Data::WeaponInfos >* >( pSelectedItem )->GetPointer();
        pResult = pWeapon->CreateCopy();
    }
    else
    {
        pResult = new ADN_Weapons_Data::WeaponInfos();
        pResult->bDirect_ = ammo.bDirect_.GetData() && pLauncher->bDirect_.GetData();
        pResult->bIndirect_ = ammo.bIndirect_.GetData() && pLauncher->bIndirect_.GetData();
    }

    pResult->ptrAmmunition_ = (ADN_Equipement_Data::AmmoCategoryInfo*)pAmmo;
    pResult->ptrLauncher_ = pLauncher;
    pResult->strName_ = pLauncher->strName_.GetData() + " & " + pAmmo->strName_.GetData();

    return pResult;
}
