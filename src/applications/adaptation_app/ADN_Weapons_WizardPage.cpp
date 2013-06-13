// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Weapons_WizardPage.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage constructor
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
ADN_Weapons_WizardPage::ADN_Weapons_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent /*= 0*/ )
    : ADN_WizardPage( existingItems, pageTitle, pParent )
    , pComboLauncher_( 0 )
    , pComboAmmo_( 0 )
        
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage destructor
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
ADN_Weapons_WizardPage::~ADN_Weapons_WizardPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage::CreateNameField
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
QWidget* ADN_Weapons_WizardPage::CreateNameField()
{
    QWidget* result = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout( result );

    pComboLauncher_ = new QComboBox();
    layout->addWidget( pComboLauncher_ );
    pComboAmmo_ = new QComboBox();
    layout->addWidget( pComboAmmo_ );

    ADN_Launchers_Data::T_LauncherInfos_Vector& vLaunchers = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos();
    for( ADN_Launchers_Data::IT_LauncherInfos_Vector itLauncher = vLaunchers.begin(); itLauncher != vLaunchers.end(); ++itLauncher )
        pComboLauncher_->insertItem( (*itLauncher)->strName_.GetData().c_str() );
    pComboLauncher_->model()->sort( 0 );

    ADN_Equipement_Data::ResourceInfos& ammoDotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition );
    ADN_Equipement_Data::T_CategoryInfos_Vector& vAmmo = ammoDotation.GetCategories();
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector itAmmo = vAmmo.begin(); itAmmo != vAmmo.end(); ++itAmmo )
        pComboAmmo_->insertItem( (*itAmmo)->strName_.GetData().c_str() );
    pComboAmmo_->model()->sort( 0 );

    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage::GetName
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
std::string ADN_Weapons_WizardPage::GetName()
{
    return QString( "%1 & %2").arg( pComboLauncher_->currentText() ).arg( pComboAmmo_->currentText() ).toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage::validatePage
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
bool ADN_Weapons_WizardPage::validatePage()
{
    std::string strLauncher = pComboLauncher_->currentText().toAscii().constData();
    std::string strAmmo = pComboAmmo_->currentText().toAscii().constData();

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
            tools::translate( "ADN_Weapons_WizardPage", "Warning" ),
            tools::translate( "ADN_Weapons_WizardPage", "The weapon and ammunition are not compatible (direct/indirect). Please modify your choices." ),
            QMessageBox::Ok, QMessageBox::NoButton );
        return false;
    }
    return ADN_WizardPage::validatePage();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_WizardPage::ApplyOptions
// Created: LDC 2012-09-24
// -----------------------------------------------------------------------------
void ADN_Weapons_WizardPage::ApplyOptions()
{
    if( element_ )
    {
        std::string strLauncher = pComboLauncher_->currentText().toAscii().constData();
        ADN_Launchers_Data::LauncherInfos* pLauncher = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().FindLauncher( strLauncher );
        element_->ptrLauncher_ = pLauncher;
        
        std::string strAmmo = pComboAmmo_->currentText().toAscii().constData();
        ADN_Equipement_Data::ResourceInfos& ammoDotation = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition );
        ADN_Equipement_Data::CategoryInfo* pAmmo = ammoDotation.FindCategory( strAmmo );
        ADN_Equipement_Data::AmmoCategoryInfo* ammo = dynamic_cast< ADN_Equipement_Data::AmmoCategoryInfo* >( pAmmo );        
        element_->ptrAmmunition_ = ammo;

        element_->ConnectLauncherAmmunition();
    }
}
