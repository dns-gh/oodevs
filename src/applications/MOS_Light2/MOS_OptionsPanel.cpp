// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-09 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_OptionsPanel.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:00 $
// $Revision: 4 $
// $Workfile: MOS_OptionsPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_OptionsPanel.h"
#include "moc_MOS_OptionsPanel.cpp"

#include "MOS_MainWindow.h"
#include "MOS_Options.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"

// -----------------------------------------------------------------------------
// Name: MOS_OptionsPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
MOS_OptionsPanel::MOS_OptionsPanel( QWidget* pParent )
    : QWidget( pParent )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();

    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 5 );
    pLayout->setSpacing( 5 );

    QTabWidget* pTabWidget = new QTabWidget( this );
    pLayout->addWidget( pTabWidget );

    // Checkpoint panel
    QWidget* pCheckpointPanel = new QWidget( pTabWidget );
    pTabWidget->addTab( pCheckpointPanel, tr( "Checkpoints" ) );
    QGridLayout* pSubLayout1 = new QGridLayout( pCheckpointPanel, 2, 2, 5 );
    pSubLayout1->setMargin( 5 );

    QLabel* pL1 = new QLabel( tr( "Fréquence:" ), pCheckpointPanel );
    pCheckpointSpinbox_ = new QSpinBox( 0, 100, 1, pCheckpointPanel );
    pCheckpointSpinbox_->setValue( 10 );

    QGroupBox* pBG1 = new QGroupBox( 2, Qt::Horizontal, tr( "Sauvegarde immédiate" ), pCheckpointPanel );
    pCheckpointNameEdit_ = new QLineEdit( pBG1 );
    QPushButton* pCheckpointButton = new QPushButton( tr( "Sauvegarder" ), pBG1 );
    
    pSubLayout1->addWidget( pL1, 0, 0 );
    pSubLayout1->addWidget( pCheckpointSpinbox_, 0, 1 );
    pSubLayout1->addMultiCellWidget( pBG1, 1, 1, 0, 1 ); 
    pSubLayout1->setRowStretch( 2, 10 );

    // Display panel
    QWidget* pDisplayPanel = new QWidget( pTabWidget );
    pTabWidget->addTab( pDisplayPanel, tr( "Affichage" ) );
    QGridLayout* pSubLayout2 = new QGridLayout( pDisplayPanel, 7, 2, 5 );
    pSubLayout2->setMargin( 5 );

    QLabel* pL2 = new QLabel( tr( "Taille police" ), pDisplayPanel );
    pFontSpinbox_ = new QSpinBox( 1, 50, 1, pDisplayPanel );
    pFontSpinbox_->setValue( options.nFontSize_ );

    pSubLayout2->addWidget( pL2, 1, 0 );
    pSubLayout2->addWidget( pFontSpinbox_, 1, 1 );
    pSubLayout2->setRowStretch( 7, 10 );

    pDrawObjectIcons_ = new QCheckBox( tr( "Afficher les icones des objets"), pDisplayPanel );
    pDrawObjectIcons_->setChecked( options.bDrawObjetIcons_ );
    pSubLayout2->addWidget( pDrawObjectIcons_, 2, 0 );

    pDrawHoveredInfo_ = new QCheckBox( tr( "Afficher des informations au survol des pions" ), pDisplayPanel );
    pDrawHoveredInfo_->setChecked( options.bDisplayHoveredInfo_ );
    pSubLayout2->addWidget( pDrawHoveredInfo_, 3, 0 );

    pDisplayRCOnMap_ = new QCheckBox( tr("Afficher les RC sur la carte"), pDisplayPanel );
    pDisplayRCOnMap_->setChecked( options.bDisplayRConMap_ );
    pSubLayout2->addWidget( pDisplayRCOnMap_, 4, 0 );

    pDisplayMessagesOnMap_ = new QCheckBox( tr("Afficher aussi les messages"), pDisplayPanel );
    pDisplayMessagesOnMap_->setChecked( options.bDisplayMessagesOnMap_ );
    pSubLayout2->addWidget( pDisplayMessagesOnMap_, 5, 0 );

    pDisplayTracesOnMap_ = new QCheckBox( tr("Afficher aussi les traces"), pDisplayPanel );
    pDisplayTracesOnMap_->setChecked( options.bDisplayTracesOnMap_ );
    pSubLayout2->addWidget( pDisplayTracesOnMap_, 6, 0 );

    // Other panel
    QWidget* pOtherPanel = new QWidget( pTabWidget );
    pTabWidget->addTab( pOtherPanel, tr( "Autres" ) );
    QGridLayout* pSubLayout3 = new QGridLayout( pOtherPanel );
    pSubLayout3->setMargin( 5 );
    pAutoOpenCheckbox_ = new QCheckBox( tr( "Ouverture arbre auto" ), pOtherPanel );
    pAutoOpenCheckbox_->setChecked( options.bOpenTreeToItem_ );
    pAutoSaveLoadCheckbox_ = new QCheckBox( tr( "Lecture/sauvegarde lima/limites" ), pOtherPanel );
    pAutoSaveLoadCheckbox_->setChecked( options.bSaveLoadTacticalLines_ );
    pSubLayout3->addMultiCellWidget( pAutoOpenCheckbox_, 0, 0, 0, 0 );
    pSubLayout3->addMultiCellWidget( pAutoOpenCheckbox_, 0, 0, 1, 1 );
    pSubLayout3->setRowStretch( 1, 10 );

    // Ok Apply Cancel buttons
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pLayout );
    pButtonLayout->setSpacing( 5 );
    QPushButton* pOkButton = new QPushButton( tr( "OK" ), this );
    QPushButton* pApplyButton = new QPushButton( tr( "Appliquer" ), this );
    QPushButton* pCancelButton = new QPushButton( tr( "Annuler" ), this );
    pButtonLayout->addStretch( 10 );
    pButtonLayout->addWidget( pOkButton, 0, Qt::AlignRight );
    pButtonLayout->addWidget( pApplyButton, 0, Qt::AlignRight );
    pButtonLayout->addWidget( pCancelButton, 0, Qt::AlignRight );
    pOkButton->setDefault( true );

    connect( pCheckpointButton, SIGNAL( clicked() ), this, SLOT( OnCheckpoint() ) );
    connect( pOkButton, SIGNAL( clicked() ), this, SLOT( Apply() ) );
    connect( pOkButton, SIGNAL( clicked() ), parentWidget(), SLOT( hide() ) );
    connect( pApplyButton, SIGNAL( clicked() ), this, SLOT( Apply() ) );
    connect( pCancelButton, SIGNAL( clicked() ), parentWidget(), SLOT( hide() ) );
    connect( pDisplayRCOnMap_, SIGNAL( clicked() ), this, SLOT( OnDisplayRC() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_OptionsPanel destructor
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
MOS_OptionsPanel::~MOS_OptionsPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_OptionsPanel::OnCheckpoint
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
void MOS_OptionsPanel::OnCheckpoint()
{
    MOS_ASN_MsgCtrlCheckPointSaveNow asnMsg;

    if( !pCheckpointNameEdit_->text().isEmpty() )
    {
        asnMsg.GetAsnMsg().m.nomPresent = 1;
        asnMsg.GetAsnMsg().nom = pCheckpointNameEdit_->text();
    }
    asnMsg.Send();
}


// -----------------------------------------------------------------------------
// Name: MOS_OptionsPanel::Apply
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
void MOS_OptionsPanel::Apply()
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    MOS_ASN_MsgCtrlCheckPointSetFrequency asnMsg;
    asnMsg.GetAsnMsg() = pCheckpointSpinbox_->value();
    asnMsg.Send();

    options.nFontSize_ = pFontSpinbox_->value();

    options.bOpenTreeToItem_ = pAutoOpenCheckbox_->isChecked();
    options.bSaveLoadTacticalLines_ = pAutoSaveLoadCheckbox_->isChecked();
    options.bDrawObjetIcons_ = pDrawObjectIcons_->isChecked();
    options.bDisplayHoveredInfo_ = pDrawHoveredInfo_->isChecked();
    options.bDisplayRConMap_ = pDisplayRCOnMap_->isChecked();
    options.bDisplayMessagesOnMap_ = pDisplayMessagesOnMap_->isChecked();
    options.bDisplayTracesOnMap_ = pDisplayTracesOnMap_->isChecked();

}

// -----------------------------------------------------------------------------
// Name: MOS_OptionsPanel::OnDisplayRC
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
void MOS_OptionsPanel::OnDisplayRC()
{
    if( pDisplayRCOnMap_->isChecked() )
    {
        pDisplayMessagesOnMap_->setEnabled( true );
        pDisplayTracesOnMap_->setEnabled( true );
    }
    else
    {
        pDisplayMessagesOnMap_->setEnabled( false );
        pDisplayTracesOnMap_->setEnabled( false );
    }
}