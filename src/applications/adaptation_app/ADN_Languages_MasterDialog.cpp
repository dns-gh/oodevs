// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Languages_MasterDialog.h"
#include "moc_ADN_Languages_MasterDialog.cpp"
#include "ADN_Gui_Tools.h"
#include "ADN_Languages_Data.h"
#include "ADN_Workspace.h"
#include "tools/Languages.h"

// -----------------------------------------------------------------------------
// Name: ADN_Languages_MasterDialog constructor
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
ADN_Languages_MasterDialog::ADN_Languages_MasterDialog( ADN_Languages_Data& data )
    : gui::RichWidget< QDialog >( "language-master-dialog", 0 )
    , data_( data )
{
    gui::ObjectNameManager::getInstance()->AddSubLevel( "language-master-dialog" );

    setFixedSize( 240, 140 );
    setCaption( tr( "Master language editor" ) );

    QGroupBox* masterGroupBox = new QGroupBox( tr( "Choose a master language" ) );
    QHBoxLayout* masterLayout = new QHBoxLayout( masterGroupBox );
    combo_= new gui::RichWidget< QComboBox >( "combo" );
    combo_->setModel( &model_ );
    masterLayout->addWidget( combo_, 1, Qt::AlignCenter );

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QPushButton* cancelButton = ADN_Gui_Tools::CreatePushButton( "cancel", tr( "Cancel" ), qApp->style()->standardIcon( QStyle::SP_DialogCancelButton ), this, SLOT( reject() ) );
    QPushButton* okButton = ADN_Gui_Tools::CreatePushButton( "ok", tr( "Ok" ), qApp->style()->standardIcon( QStyle::SP_DialogOkButton ), this, SLOT( accept() ) );
    buttonsLayout->addWidget( okButton );
    buttonsLayout->addStretch( 1 );
    buttonsLayout->addWidget( cancelButton );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( masterGroupBox );
    mainLayout->addLayout( buttonsLayout );

    gui::ObjectNameManager::getInstance()->RemoveSubLevel();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_MasterDialog destructor
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
ADN_Languages_MasterDialog::~ADN_Languages_MasterDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_MasterDialog::showEvent
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_MasterDialog::showEvent( QShowEvent* pEvent )
{
    if( !data_.IsMasterEmpty() )
        return;

    model_.clear();
    QString english;
    const tools::LanguagesVector& languages = data_.GetAllLanguages().GetLanguages();
    for( auto it = languages.begin(); it != languages.end(); ++it )
    {
        if( it->GetCode() == "en" )
            english = it->GetName().c_str();
        model_.appendRow( QList< QStandardItem* >()
                          << new QStandardItem( it->GetName().c_str() )
                          << new QStandardItem( it->GetCode().c_str() ) );
    }
    model_.sort( 1 );
    if( !english.isEmpty() )
        combo_->setCurrentIndex( combo_->findText( english ) );
    QDialog::showEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_MasterDialog::accept
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_MasterDialog::accept()
{
    const QString selectedLanguage = combo_->currentText();
    const tools::LanguagesVector& languages = data_.GetAllLanguages().GetLanguages();
    for( auto it = languages.begin(); it != languages.end(); ++it )
        if( it->GetName() == selectedLanguage.toStdString() )
        {
            data_.SetMaster( it->GetCode() );
            emit MasterChanged();
            ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
            QDialog::accept();
            return;
        }
    QDialog::reject();
    throw MASA_EXCEPTION( "Can't found language: " + selectedLanguage.toStdString() );
}
