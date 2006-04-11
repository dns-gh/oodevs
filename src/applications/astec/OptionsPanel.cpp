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
// $Archive: /MVW_v10/Build/SDK/Light2/src/OptionsPanel.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:00 $
// $Revision: 4 $
// $Workfile: OptionsPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "OptionsPanel.h"
#include "moc_OptionsPanel.cpp"

#include "MainWindow.h"
#include "Options.h"
#include "ASN_Messages.h"
#include "ASN_Types.h"

// -----------------------------------------------------------------------------
// Name: OptionsPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
OptionsPanel::OptionsPanel( QWidget* pParent )
    : QWidget( pParent )
{
    Options& options = App::GetApp().GetOptions();

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
    QGridLayout* pSubLayout2 = new QGridLayout( pDisplayPanel, 9, 2, 5 );
    pSubLayout2->setMargin( 5 );

    QLabel* pL2 = new QLabel( tr( "Taille police" ), pDisplayPanel );
    pFontSpinbox_ = new QSpinBox( 1, 50, 1, pDisplayPanel );
    pFontSpinbox_->setValue( options.nFontSize_ );

    pSubLayout2->addWidget( pL2, 1, 0 );
    pSubLayout2->addWidget( pFontSpinbox_, 1, 1 );
    pSubLayout2->setRowStretch( 9, 10 );

    pDrawObjectIcons_ = new QCheckBox( tr( "Afficher les icones des objets"), pDisplayPanel );
    pDrawObjectIcons_->setChecked( options.bDrawObjetIcons_ );
    pSubLayout2->addWidget( pDrawObjectIcons_, 2, 0 );

    pDrawHoveredInfo_ = new QCheckBox( tr( "Afficher des informations au survol des pions" ), pDisplayPanel );
    pDrawHoveredInfo_->setChecked( options.bDisplayHoveredInfo_ );
    pSubLayout2->addWidget( pDrawHoveredInfo_, 3, 0 );

    pDisplayRCOnMap_ = new QCheckBox( tr("Afficher les RC sur la carte"), pDisplayPanel );
    pDisplayRCOnMap_->setChecked( options.bDisplayRConMap_ );
    pSubLayout2->addWidget( pDisplayRCOnMap_, 4, 0 );

    pDisplayOnlySubscribedAgentsRC_ = new QCheckBox( tr("Afficher uniquement les RC des unités abonnées"), pDisplayPanel );
    pDisplayOnlySubscribedAgentsRC_->setChecked( options.bDisplayOnlySubscribedAgentsRC_ );
    pSubLayout2->addWidget( pDisplayOnlySubscribedAgentsRC_, 5, 0 );

    pDisplayMessagesOnMap_ = new QCheckBox( tr("Afficher aussi les messages"), pDisplayPanel );
    pDisplayMessagesOnMap_->setChecked( options.bDisplayMessagesOnMap_ );
    pSubLayout2->addWidget( pDisplayMessagesOnMap_, 6, 0 );

    pDisplayTracesOnMap_ = new QCheckBox( tr("Afficher aussi les traces"), pDisplayPanel );
    pDisplayTracesOnMap_->setChecked( options.bDisplayTracesOnMap_ );
    pSubLayout2->addWidget( pDisplayTracesOnMap_, 7, 0 );

    pDisplayIdentificationLevelOnMap_ = new QCheckBox( tr("Afficher aussi les RC d'identification"), pDisplayPanel );
    pDisplayIdentificationLevelOnMap_->setChecked( options.bDisplayIdentificationLevelOnMap_ );
    pSubLayout2->addWidget( pDisplayIdentificationLevelOnMap_, 8, 0 );

    // Other panel
    QWidget* pOtherPanel = new QWidget( pTabWidget );
    pTabWidget->addTab( pOtherPanel, tr( "Autres" ) );
    QGridLayout* pSubLayout3 = new QGridLayout( pOtherPanel );
    pSubLayout3->setMargin( 5 );
    pAutoOpenCheckbox_ = new QCheckBox( tr( "Ouverture arbre auto" ), pOtherPanel );
    pAutoOpenCheckbox_->setChecked( options.bOpenTreeToItem_ );
    pAutoSaveLoadCheckbox_ = new QCheckBox( tr( "Lecture/sauvegarde lima/limites" ), pOtherPanel );
    pAutoSaveLoadCheckbox_->setChecked( options.bSaveLoadTacticalLines_ );
    pAskForTacticalLineSavingOnExit_ = new QCheckBox( tr( "Confirmer sauvegarde lima/limites" ), pOtherPanel );
    pAskForTacticalLineSavingOnExit_->setChecked( options.bAskForTacticalLineSavingOnExit_ );
    pSubLayout3->addMultiCellWidget( pAutoOpenCheckbox_              , 0, 0, 0, 0 );
    pSubLayout3->addMultiCellWidget( pAutoSaveLoadCheckbox_          , 1, 1, 0, 0 );
    pSubLayout3->addMultiCellWidget( pAskForTacticalLineSavingOnExit_, 2, 2, 0, 0 );
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
// Name: OptionsPanel destructor
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
OptionsPanel::~OptionsPanel()
{
}


// -----------------------------------------------------------------------------
// Name: OptionsPanel::OnCheckpoint
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
void OptionsPanel::OnCheckpoint()
{
    ASN_MsgCtrlCheckPointSaveNow asnMsg;

    if( !pCheckpointNameEdit_->text().isEmpty() )
    {
        asnMsg.GetAsnMsg().m.nomPresent = 1;
        asnMsg.GetAsnMsg().nom = pCheckpointNameEdit_->text();
    }
    asnMsg.Send();
}


// -----------------------------------------------------------------------------
// Name: OptionsPanel::Apply
// Created: APE 2004-07-09
// -----------------------------------------------------------------------------
void OptionsPanel::Apply()
{
    Options& options = App::GetApp().GetOptions();
    ASN_MsgCtrlCheckPointSetFrequency asnMsg;
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
    options.bDisplayIdentificationLevelOnMap_ = pDisplayIdentificationLevelOnMap_->isChecked();
    options.bDisplayOnlySubscribedAgentsRC_ = pDisplayOnlySubscribedAgentsRC_->isChecked();
    options.bAskForTacticalLineSavingOnExit_ = pAskForTacticalLineSavingOnExit_->isChecked();
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::OnDisplayRC
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
void OptionsPanel::OnDisplayRC()
{
    if( pDisplayRCOnMap_->isChecked() )
    {
        pDisplayMessagesOnMap_->setEnabled( true );
        pDisplayTracesOnMap_->setEnabled( true );
        pDisplayIdentificationLevelOnMap_->setEnabled( true );
        pDisplayOnlySubscribedAgentsRC_->setEnabled( true );
    }
    else
    {
        pDisplayMessagesOnMap_->setEnabled( false );
        pDisplayTracesOnMap_->setEnabled( false );
        pDisplayIdentificationLevelOnMap_->setEnabled( false );
        pDisplayOnlySubscribedAgentsRC_->setEnabled( false );
    }
}