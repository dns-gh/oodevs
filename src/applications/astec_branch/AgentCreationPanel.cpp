// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationPanel.cpp $
// $Author: Age $
// $Modtime: 14/04/05 11:06 $
// $Revision: 9 $
// $Workfile: AgentCreationPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentCreationPanel.h"
#include "moc_AgentCreationPanel.cpp"

#include "App.h"
#include "MainWindow.h"
#include "Agent.h"
#include "Gtia.h"
#include "Team.h"
#include "AgentManager.h"
#include "AgentCreationListView.h"
#include "AgentCreationMapFilter.h"
#include "Tools.h"
#include "ActionContext.h"
#include "Nature.h"
#include "ChangeLogisticLinksDialog.h"
#include "LogisticSupplyChangeQuotasDialog.h"

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationPanel::AgentCreationPanel( QWidget* pParent )
    : QWidgetStack( pParent )
    , pMapFilter_ ( new AgentCreationMapFilter( *this ) )
{
    this->Initialize();

    // Page that displays a message
    pMessagePanel_ = new QWidget( this );

    QGridLayout* pLayout = new QGridLayout( pMessagePanel_, 1, 1, 5 );
    pLayout->setAutoAdd( true );

    QLabel* pMessageLabel = new QLabel( tr( "S�lectionnez un camp, GTIA ou automate" ), pMessagePanel_ );
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
    QPushButton* pCreateGtiaButton = new QPushButton( tr( "Cr�er GTIA" ), pFrame );

    this->addWidget( pGtiaCreationPanel_ );

    // Page that appears when a gtia or agent is selected
    pAgentListPanel_ = new QWidget( this );

    QGridLayout* pLayout4 = new QGridLayout( pAgentListPanel_, 2, 2, 5 );

    pAgentListView_ = new AgentCreationListView( pAgentListPanel_ );
    QLabel* pRadiusLabel = new QLabel( tr("Rayon de dispersion"), pAgentListPanel_ );
    pRadiusSpinBox_ = new QSpinBox( 0, 9999, 50, pAgentListPanel_ );
    pRadiusSpinBox_->setSuffix( "m" );

    pLayout4->addMultiCellWidget( pAgentListView_, 0, 0, 0, 1 );
    pLayout4->addWidget( pRadiusLabel, 1, 0 );
    pLayout4->addWidget( pRadiusSpinBox_, 1, 1 );

    this->addWidget( pAgentListPanel_ );

    this->raiseWidget( pMessagePanel_ );

    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), this,   SLOT( FillRemotePopupMenu( QPopupMenu&, const ActionContext& ) ) );

    connect( pCreateGtiaButton, SIGNAL( clicked() ), this, SLOT( CreateGtia() ) );

    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ), &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );

    pMapFilter_->Push();
}


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationPanel::~AgentCreationPanel()
{
}


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void AgentCreationPanel::SetSelectedElement( SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;

    if( selectedElement.pAgent_ != 0 && selectedElement.pAgent_->IsAutomate() )
    {
        IT_AgentTypeVector it = std::find_if( automateTypes_.begin(), automateTypes_.end(), AgentType_Cmp( selectedElement_.pAgent_->GetCategory() ) );
        
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
// Name: AgentCreationPanel::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void AgentCreationPanel::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 || ! context.selectedElement_.pAgent_->IsAutomate() )
        return;

    selectedElement_ = context.selectedElement_;
    popupMenu.insertItem( selectedElement_.pAgent_->IsEmbraye() ? tr( "D�brayer automate" ) : tr( "Embrayer automate" ), this, SLOT( ToggleAutomate() ) );
    popupMenu.insertItem( selectedElement_.pAgent_->IsAggregated() ? tr( "D�saggr�ger l'automate" ) : tr( "Aggr�ger l'automate" ), this, SLOT( ToggleAggregation() ) );
    popupMenu.insertItem( "Liens Logistiques" , this , SLOT( LogisticLinksDialog() ) );
    if ( selectedElement_.pAgent_->IsLogisticRavitaillement() 
        && ( selectedElement_.pAgent_->IsLogisticBLT() || selectedElement_.pAgent_->IsLogisticBLD() ) )
        popupMenu.insertItem( "Changer les quotas", this, SLOT( ChangeQuotasDialog() ) );
}


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::CreateGtia
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
void AgentCreationPanel::CreateGtia()
{
    assert( selectedElement_.pTeam_ != 0 );
    // $$$$ AGE 2006-02-13: ....
    Gtia* pNewGtia = new Gtia( true );
    pNewGtia->SetType( pGtiaTypeCombo_->currentText().ascii() );

    selectedElement_.pTeam_->RegisterGtia( *pNewGtia );

    App::GetApp().NotifyGtiaCreated( *pNewGtia );
    SelectedElement message( *pNewGtia );
    emit ElementSelected( message );
}


// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::ToggleAutomate
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
void AgentCreationPanel::ToggleAutomate()
{
    if( selectedElement_.pAgent_ == 0 || ! selectedElement_.pAgent_->IsAutomate() )
        return;

    selectedElement_.pAgent_->SetEmbraye( ! selectedElement_.pAgent_->IsEmbraye() );
    App::GetApp().NotifyAgentUpdated( *selectedElement_.pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::ToggleAggregation
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void AgentCreationPanel::ToggleAggregation()
{
    if( selectedElement_.pAgent_->IsAggregated() )
        selectedElement_.pAgent_->SetAggregation( false );
    else
        selectedElement_.pAgent_->SetAggregation( true );
}

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::LogisticLinksDialog
// Created: HME 2005-08-2
// -----------------------------------------------------------------------------
void AgentCreationPanel::LogisticLinksDialog()
{
    static ChangeLogisticLinksDialog* pDialog = new ChangeLogisticLinksDialog( this );
    pDialog->SetAgent( *(selectedElement_.pAgent_) );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::ChangeQuotasDialog
// Created: HME 2005-11-03
// -----------------------------------------------------------------------------
void AgentCreationPanel::ChangeQuotasDialog()
{
    static LogisticSupplyChangeQuotasDialog* pDialog = new LogisticSupplyChangeQuotasDialog( this );
    pDialog->SetAgent( *(selectedElement_.pAgent_) );
    pDialog->show();
}

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::Initialize
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
void AgentCreationPanel::Initialize()
{
    const std::string   strCurrentDir = MT_GetCurrentDir();

    std::string         strDir;
    std::string         strFile;
    MT_ExtractFilePath( App::GetApp().GetRootConfigFile(), strDir  );
    MT_ExtractFileName( App::GetApp().GetRootConfigFile(), strFile );

    MT_ChangeDir      ( strDir );

    InputArchive scipioArchive;
    try
    {
        scipioArchive.Open( strFile );
        scipioArchive.Section( "Scipio" );
        scipioArchive.Section( "Donnees" );

        std::string strUnitFilename;
        std::string strAutomataFilename;
        std::string strGtiaFilename;
        scipioArchive.ReadField( "Pions", strUnitFilename );
        scipioArchive.ReadField( "Automates", strAutomataFilename );
        scipioArchive.ReadField( "GroupesConnaissance", strGtiaFilename );

        InputArchive unitArchive;
        unitArchive.Open( strUnitFilename );

        unitArchive.BeginList( "Pions" );
        while( unitArchive.NextListElement() )
        {
            unitArchive.Section( "Unite" );

            std::string strName;
            unitArchive.ReadAttribute( "nom", strName );

            AgentType* pUnitType = new AgentType();
            pUnitType->strName_ = strName;
            pUnitType->pNature_ = new Nature( unitArchive );
            unitTypes_.push_back( pUnitType );

            unitArchive.EndSection(); // Unite
        }
        unitArchive.EndList();
        unitArchive.Close();

        InputArchive automataArchive;
        automataArchive.Open( strAutomataFilename );

        automataArchive.BeginList( "Automates" );
        while( automataArchive.NextListElement() )
        {
            automataArchive.Section( "Unite" );

            std::string strName;
            automataArchive.ReadAttribute( "nom", strName );
            AgentType* pAutomateType = new AgentType();
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
                automataArchive.Read( strRegExp );
                automataArchive.EndSection();

                // We are given either a number of units, or a reg exp for that number.
                // Out of simplicity, if it's a reg exp, we'll assume the number to be 1.
                QString strRegExp2( strRegExp.c_str() );
                bool bIsNbr = false;
                int nNbr = strRegExp2.toInt( &bIsNbr );
                if( !bIsNbr )
                    nNbr = 1;
                
                IT_AgentTypeVector it = std::find_if( unitTypes_.begin(), unitTypes_.end(), AgentType_Cmp( strElement ) );
                assert( it != unitTypes_.end() );
                for( int n = 0; n < nNbr; ++n )
                    pAutomateType->composition_.push_back( *it );
            }
            automataArchive.EndList();      // Constitution
            automataArchive.EndSection();   // Automate

            automataArchive.Section( "PionPC" );
            std::string strPCType;
            automataArchive.ReadAttribute( "type", strPCType );
            IT_AgentTypeVector it = std::find_if( unitTypes_.begin(), unitTypes_.end(), AgentType_Cmp( strPCType ) );
            if( it != unitTypes_.end() )
                pAutomateType->pNature_ = new Nature( *(*it)->pNature_ );
            automataArchive.EndSection(); // PionPC

            automataArchive.EndSection();   // Unite
        }
        automataArchive.EndList();    
        automataArchive.Close();

        InputArchive gtiaArchive;
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
    catch( ... )
    {
        throw;
    }

    MT_ChangeDir( strCurrentDir );
}

// -----------------------------------------------------------------------------
// Name: AgentCreationPanel::AgentType destructor
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
AgentCreationPanel::AgentType::~AgentType()
{
    delete pNature_;
}
