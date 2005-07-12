// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationPanel.cpp $
// $Author: Age $
// $Modtime: 14/04/05 11:06 $
// $Revision: 9 $
// $Workfile: MOS_AgentCreationPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_AgentCreationPanel.h"
#include "moc_MOS_AgentCreationPanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_AgentManager.h"
#include "MOS_AgentCreationListView.h"
#include "MOS_AgentCreationMapFilter.h"
#include "MOS_Tools.h"
#include "MOS_ActionContext.h"
#include "MOS_Nature.h"


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationPanel::MOS_AgentCreationPanel( QWidget* pParent )
    : QWidgetStack( pParent )
    , pMapFilter_ ( new MOS_AgentCreationMapFilter( *this ) )
{
    this->Initialize();

    // Page that displays a message
    pMessagePanel_ = new QWidget( this );

    QGridLayout* pLayout = new QGridLayout( pMessagePanel_, 1, 1, 5 );
    pLayout->setAutoAdd( true );

    QLabel* pMessageLabel = new QLabel( tr( "Sélectionnez un camp, GTIA ou automate" ), pMessagePanel_ );
    pMessageLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pMessageLabel->setAlignment( AlignCenter );

    this->addWidget( pMessagePanel_ );

    // Page that appears when a team is selected
    pGtiaCreationPanel_ = new QWidget( this );

    QGridLayout* pLayout2 = new QGridLayout( pGtiaCreationPanel_, 1, 1, 5 );
    pLayout2->setAutoAdd( true );

    QFrame* pFrame = new QFrame( pGtiaCreationPanel_ );
    pFrame->setFrameStyle( QFrame::Box | QFrame::Sunken );

    QGridLayout* pLayout3 = new QGridLayout( pFrame, 1, 1, 5 );
    pLayout3->setAutoAdd( true );

    pGtiaTypeCombo_ = new QComboBox( pFrame );
    for( IT_GtiaTypeVector it = gtiaTypes_.begin(); it != gtiaTypes_.end(); ++it )
        pGtiaTypeCombo_->insertItem( (*it).c_str() );
    QPushButton* pCreateGtiaButton = new QPushButton( tr( "Créer GTIA" ), pFrame );

    this->addWidget( pGtiaCreationPanel_ );

    // Page that appears when a gtia or agent is selected
    pAgentListPanel_ = new QWidget( this );

    QGridLayout* pLayout4 = new QGridLayout( pAgentListPanel_, 2, 2, 5 );

    pAgentListView_ = new MOS_AgentCreationListView( pAgentListPanel_ );
    QLabel* pRadiusLabel = new QLabel( tr("Rayon de dispersion"), pAgentListPanel_ );
    pRadiusSpinBox_ = new QSpinBox( 0, 9999, 50, pAgentListPanel_ );
    pRadiusSpinBox_->setSuffix( "m" );

    pLayout4->addMultiCellWidget( pAgentListView_, 0, 0, 0, 1 );
    pLayout4->addWidget( pRadiusLabel, 1, 0 );
    pLayout4->addWidget( pRadiusSpinBox_, 1, 1 );

    this->addWidget( pAgentListPanel_ );

    this->raiseWidget( pMessagePanel_ );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );

    connect( pCreateGtiaButton, SIGNAL( clicked() ), this, SLOT( CreateGtia() ) );

    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );

    pMapFilter_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationPanel::~MOS_AgentCreationPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void MOS_AgentCreationPanel::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;

    if( selectedElement.pAgent_ != 0 && selectedElement.pAgent_->IsAutomate() )
    {
        IT_AgentTypeVector it = std::find_if( automateTypes_.begin(), automateTypes_.end(), MOS_AgentType_Cmp( selectedElement_.pAgent_->GetCategory() ) );
        
        if( it != automateTypes_.end() )
            pAgentListView_->FillList( (*it)->composition_ );
        else
            pAgentListView_->ShowInvalid();
        this->raiseWidget( pAgentListPanel_ );
        return;
    }

    if( selectedElement.pGtia_ != 0 )
    {
        pAgentListView_->FillList( automateTypes_ );
        this->raiseWidget( pAgentListPanel_ );
        return;
    }

    if( selectedElement.pTeam_ != 0 )
    {
        this->raiseWidget( pGtiaCreationPanel_ );
        return;
    }

    this->raiseWidget( pMessagePanel_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_AgentCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 || ! context.selectedElement_.pAgent_->IsAutomate() )
        return;

    selectedElement_ = context.selectedElement_;
    popupMenu.insertItem( selectedElement_.pAgent_->IsEmbraye() ? tr( "Débrayer automate" ) : tr( "Embrayer automate" ), this, SLOT( ToggleAutomate() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::CreateGtia
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
void MOS_AgentCreationPanel::CreateGtia()
{
    assert( selectedElement_.pTeam_ != 0 );

    MOS_Gtia* pNewGtia = new MOS_Gtia( true );
    pNewGtia->SetType( pGtiaTypeCombo_->currentText().ascii() );

    selectedElement_.pTeam_->RegisterGtia( *pNewGtia );

    MOS_App::GetApp().NotifyGtiaCreated( *pNewGtia );
    emit ElementSelected( MOS_SelectedElement( *pNewGtia ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::ToggleAutomate
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void MOS_AgentCreationPanel::ToggleAutomate()
{
    if( selectedElement_.pAgent_ == 0 || ! selectedElement_.pAgent_->IsAutomate() )
        return;

    selectedElement_.pAgent_->SetEmbraye( ! selectedElement_.pAgent_->IsEmbraye() );
    MOS_App::GetApp().NotifyAgentUpdated( *selectedElement_.pAgent_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::Initialize
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void MOS_AgentCreationPanel::Initialize()
{
    //$$$ Hard coded file location!
    //$$$ No try catch!

    MT_XXmlInputArchive scipioArchive;
    scipioArchive.EnableExceptions( true );
    scipioArchive.Open( "./scipio.xml" );
    scipioArchive.Section( "Scipio" );
    scipioArchive.Section( "Donnees" );

    std::string strUnitFilename;
    std::string strAutomataFilename;
    std::string strGtiaFilename;
    scipioArchive.ReadField( "Pions", strUnitFilename );
    scipioArchive.ReadField( "Automates", strAutomataFilename );
    scipioArchive.ReadField( "GroupesConnaissance", strGtiaFilename );

    MT_XXmlInputArchive unitArchive;
    unitArchive.EnableExceptions( true );
    unitArchive.Open( strUnitFilename );

    unitArchive.BeginList( "Pions" );
    while( unitArchive.NextListElement() )
    {
        unitArchive.Section( "Unite" );

        std::string strName;
        unitArchive.ReadAttribute( "nom", strName );

        MOS_AgentType* pUnitType = new MOS_AgentType();
        pUnitType->strName_ = strName;
        pUnitType->pNature_ = new MOS_Nature( unitArchive );
        unitTypes_.push_back( pUnitType );

        unitArchive.EndSection(); // Unite
    }
    unitArchive.EndList();
    unitArchive.Close();

    MT_XXmlInputArchive automataArchive;
    automataArchive.EnableExceptions( true );
    automataArchive.Open( strAutomataFilename );

    automataArchive.BeginList( "Automates" );
    while( automataArchive.NextListElement() )
    {
        automataArchive.Section( "Unite" );

        std::string strName;
        automataArchive.ReadAttribute( "nom", strName );
        MOS_AgentType* pAutomateType = new MOS_AgentType();
        pAutomateType->strName_ = strName;

        automateTypes_.push_back( pAutomateType );

        automataArchive.Section( "Automate" );
        automataArchive.BeginList( "Constitution" );
        while( automataArchive.NextListElement() )
        {
            std::string strElement;
            std::string strRegExp;
            automataArchive.Section( "Pion" );
            automataArchive.ReadAttribute( "nom", strElement );
            automataArchive >> strRegExp;
            automataArchive.EndSection();

            // We are given either a number of units, or a reg exp for that number.
            // Out of simplicity, if it's a reg exp, we'll assume the number to be 1.
            QString strRegExp2( strRegExp.c_str() );
            bool bIsNbr = false;
            int nNbr = strRegExp2.toInt( &bIsNbr );
            if( !bIsNbr )
                nNbr = 1;
            
            IT_AgentTypeVector it = std::find_if( unitTypes_.begin(), unitTypes_.end(), MOS_AgentType_Cmp( strElement ) );
            assert( it != unitTypes_.end() );
            for( int n = 0; n < nNbr; ++n )
                pAutomateType->composition_.push_back( *it );
        }
        automataArchive.EndList();      // Constitution
        automataArchive.EndSection();   // Automate

        automataArchive.Section( "PionPC" );
        std::string strPCType;
        automataArchive.ReadAttribute( "type", strPCType );
        IT_AgentTypeVector it = std::find_if( unitTypes_.begin(), unitTypes_.end(), MOS_AgentType_Cmp( strPCType ) );
        if( it != unitTypes_.end() )
            pAutomateType->pNature_ = new MOS_Nature( *(*it)->pNature_ );
        automataArchive.EndSection(); // PionPC

        automataArchive.EndSection();   // Unite
    }
    automataArchive.EndList();    
    automataArchive.Close();

    MT_XXmlInputArchive gtiaArchive;
    gtiaArchive.EnableExceptions( true );
    gtiaArchive.Open( strGtiaFilename );

    gtiaArchive.BeginList( "GroupesConnaissance" );
    while( gtiaArchive.NextListElement() )
    {
        gtiaArchive.Section( "GroupeConnaissance" );
        std::string strName;
        gtiaArchive.ReadAttribute( "nom", strName );
        gtiaTypes_.push_back( strName );
        gtiaArchive.EndSection(); // GroupeConnaissance
    }
    gtiaArchive.EndList(); // GroupesConnaissance
    gtiaArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationPanel::MOS_AgentType destructor
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
MOS_AgentCreationPanel::MOS_AgentType::~MOS_AgentType()
{
    delete pNature_;
}
