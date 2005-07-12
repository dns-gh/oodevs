// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: MOS_AgentKnowledgePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_AgentKnowledgePanel.h"
#include "moc_MOS_AgentKnowledgePanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Tools.h"
#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"
#include "MOS_Gtia.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ActionContext.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentKnowledgePanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel constructor
/** @param  pParent 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
MOS_AgentKnowledgePanel::MOS_AgentKnowledgePanel( QWidget* pParent )
    : MOS_InfoPanel_ABC     ( pParent )
    , pPopupMenu_           ( new QPopupMenu( this ) )
    , pSelectedKnowledge_   ( 0 )
    , pGtia_                ( 0 )
{
    this->setMargin( 5 );
    this->setSpacing( 5 );

    pKnowledgeListView_ = new QListView( this );
    pKnowledgeListView_->addColumn( tr( "Agents connus" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    QGroupBox* pDetails = new QGroupBox( 2, Qt::Horizontal, tr( "Détails" ), this );

    new QLabel( tr( "Id:" ), pDetails );
    pIdLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Agent associé:" ), pDetails );
    pAssociatedAgentLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Position:" ), pDetails );
    pPositionLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Direction:" ), pDetails );
    pDirectionLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Vitesse:" ), pDetails );
    pSpeedLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Etat ops.:" ), pDetails );
    pStateLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Niveau de perception:" ), pDetails );
    pPerceptionLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Niveau max de perception:" ), pDetails );
    pPerceptionMaxLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Camp:" ), pDetails );
    pTeamLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Niveau:" ), pDetails );
    pLevelLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Arme:" ), pDetails );
    pWeaponLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Spécialisation:" ), pDetails );
    pSpecializationLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Qualification:" ), pDetails );
    pQualifierLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Catégorie:" ), pDetails );
    pCategoryLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Mobilité:" ), pDetails );
    pMobilityLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Capacité mission:" ), pDetails );
    pCapacityLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Rendu:" ), pDetails );
    pSurrenderedLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Fait prisonnier:" ), pDetails );
    pPrisonnerLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Réfugiés pris en compte:" ), pDetails );
    pRefugiesLabel_ = new QLabel( pDetails );

    new QLabel( tr( "PC:" ), pDetails );
    pIsHQLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Pertinence:" ), pDetails );
    pRelevanceLabel_ = new QLabel( pDetails );

    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->addColumn( tr( "Niveau perception" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );
    pPerceptionListView_->setAllColumnsShowFocus( true );

    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeCreated( MOS_Gtia&, MOS_AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeCreated( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeUpdated( MOS_Gtia&, MOS_AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeUpdated( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ),
             this,                 SLOT(    OnKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ) );

    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),         &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),             &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel destructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
MOS_AgentKnowledgePanel::~MOS_AgentKnowledgePanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnUpdate()
{
    if( GetSelectedGtia( selectedItem_ ) == 0 )
    {
        pGtia_ = 0;
        pKnowledgeListView_->clear();
        pSelectedKnowledge_ = 0;
        UpdateSelected();
    }
    else
        UpdateList();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::UpdateList
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::UpdateList()
{
    MOS_Gtia*           pGtia      = GetSelectedGtia( selectedItem_ );
    MOS_AgentKnowledge* pKnowledge = selectedItem_.pAgentKnowledge_;

    if( pGtia == 0 )
        return;

    if( pGtia_ != pGtia )
    {
        pKnowledgeListView_->clear();
        const MOS_Gtia::T_AgentKnowledgeMap& knowledges = pGtia->GetAgentKnowledges();
        for( MOS_Gtia::CIT_AgentKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pGtia->GetTeam().GetID() != (*it).second->GetRealAgent().GetTeam().GetID() )
                new MT_ValuedListViewItem<MOS_AgentKnowledge*>( (*it).second, pKnowledgeListView_, (*it).second->GetRealAgent().GetName().c_str() );
        }
        pGtia_ = pGtia;
    }

    // Try to select the appropriate knowledge.
    if( pKnowledge != 0 )
    {
        QListViewItem* pItem = pKnowledgeListView_->firstChild();
        while( pItem != 0 )
        {
            MT_ValuedListViewItem<MOS_AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_AgentKnowledge*>*)pItem;
            if( pCastItem->GetValue() == pKnowledge )
            {
                pKnowledgeListView_->setSelected( pItem, true );
                return;
            }
            pItem = pItem->nextSibling();
        }
    }

    if( pKnowledgeListView_->childCount() > 0 )
        pKnowledgeListView_->setSelected( pKnowledgeListView_->firstChild(), true );
    else
    {
        pSelectedKnowledge_ = 0;
        UpdateSelected();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::UpdateSelected
/** @param  pKnowledge 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::UpdateSelected()
{
    pIdLabel_               ->setText( "-" );
    pAssociatedAgentLabel_  ->setText( "-" );
    pPositionLabel_         ->setText( "-" );
    pDirectionLabel_        ->setText( "-" );
    pSpeedLabel_            ->setText( "-" );
    pStateLabel_            ->setText( "-" );
    pPerceptionLabel_       ->setText( "-" );
    pPerceptionMaxLabel_    ->setText( "-" );
    pTeamLabel_             ->setText( "-" );
    pLevelLabel_            ->setText( "-" );
    pWeaponLabel_           ->setText( "-" );
    pSpecializationLabel_   ->setText( "-" );
    pQualifierLabel_        ->setText( "-" );
    pCategoryLabel_         ->setText( "-" );
    pMobilityLabel_         ->setText( "-" );
    pCapacityLabel_         ->setText( "-" );
    pRefugiesLabel_         ->setText( "-" );
    pPrisonnerLabel_        ->setText( "-" );
    pSurrenderedLabel_      ->setText( "-" );
    pIsHQLabel_             ->setText( "-" );
    pRelevanceLabel_        ->setText( "-" );

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    pIdLabel_             ->setText( QString::number( pSelectedKnowledge_->GetID() ) );
    pAssociatedAgentLabel_->setText( QString::number( pSelectedKnowledge_->GetRealAgent().GetAgentID() ) );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Team ) )
        pTeamLabel_->setText(  MOS_App::GetApp().GetAgentManager().FindTeam( pSelectedKnowledge_->nTeam_)->GetName().c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Level ) )
        pLevelLabel_->setText(  ENT_Tr::ConvertFromNatureLevel( pSelectedKnowledge_->nLevel_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Weapon ) )
        pWeaponLabel_->setText(  ENT_Tr::ConvertFromUnitNatureWeapon( pSelectedKnowledge_->nWeapon_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Category ) )
        pCategoryLabel_->setText(  ENT_Tr::ConvertFromUnitNatureCategory( pSelectedKnowledge_->nCategory_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Qualifier ) )
        pQualifierLabel_->setText(  ENT_Tr::ConvertFromUnitNatureQualifier( pSelectedKnowledge_->nQualifier_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Specialization ) )
        pSpecializationLabel_->setText(  ENT_Tr::ConvertFromUnitNatureSpecialization( pSelectedKnowledge_->nSpecialization_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Mobility ) )
        pMobilityLabel_->setText(  ENT_Tr::ConvertFromUnitNatureMobility( pSelectedKnowledge_->nMobility_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Capacity ) )
        pCapacityLabel_->setText(  ENT_Tr::ConvertFromUnitCapaciteMission( pSelectedKnowledge_->nCapacity_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_IsPC ) )
        pIsHQLabel_->setText(  pSelectedKnowledge_->bIsPC_ ? tr("Oui") : tr("Non") );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_CurrentPerceptionLevel ) )
        pPerceptionLabel_->setText(  MOS_Tools::ToString( pSelectedKnowledge_->nCurrentPerceptionLevel_ ) );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_MaxPerceptionLevel ) )
        pPerceptionMaxLabel_->setText(  MOS_Tools::ToString( pSelectedKnowledge_->nMaxPerceptionLevel_ ) );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_EtatOps ) )
        pStateLabel_->setText( MT_FormatString( "%d", pSelectedKnowledge_->nEtatOps_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Speed) )
        pSpeedLabel_->setText( MT_FormatString( "%d", pSelectedKnowledge_->nSpeed_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Direction ) )
        pDirectionLabel_->setText( MT_FormatString( "%d", pSelectedKnowledge_->nDirection_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Position ) )
        pPositionLabel_->setText( pSelectedKnowledge_->strPosition_.c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Relevance ) )
        pRelevanceLabel_->setText( MT_FormatString( "%d", pSelectedKnowledge_->nRelevance_ ).c_str() );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Prisonner) )
        pPrisonnerLabel_->setText( pSelectedKnowledge_->bPrisonner_ ? tr("Oui") : tr("Non") );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Surrendered ) )
        pSurrenderedLabel_->setText( pSelectedKnowledge_->bSurrendered_ ? tr("Oui") : tr("Non") );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_Refugies ) )
        pRefugiesLabel_->setText( pSelectedKnowledge_->bRefugies_ ? tr("Oui") : tr("Non") );

    if( pSelectedKnowledge_->IsValid( MOS_AgentKnowledge::eUpdated_AutomatePerception ) )
    {
        for( MOS_AgentKnowledge::CIT_AutomatePerceptionMap it = pSelectedKnowledge_->automatePerceptionMap_.begin(); it != pSelectedKnowledge_->automatePerceptionMap_.end(); ++it )
            new QListViewItem( pPerceptionListView_, it->first->GetName().c_str(), MOS_Tools::ToString( it->second )  );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnSelectionChanged
/** @param  pItem 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_AgentKnowledge*>*)pItem;
        pSelectedKnowledge_ = pCastItem->GetValue();
        MOS_SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
        pSelectedKnowledge_ = 0;

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnRequestCenter
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;

    MT_ValuedListViewItem<MOS_AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_AgentKnowledge*>*)pItem;
    emit CenterOnPoint( pCastItem->GetValue()->GetPosition() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnKnowledgeCreated
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnKnowledgeCreated( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge )
{
    if( &gtia != selectedItem_.pGtia_ )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || gtia.GetTeam().GetID() != knowledge.GetRealAgent().GetTeam().GetID() )
        new MT_ValuedListViewItem<MOS_AgentKnowledge*>( &knowledge, pKnowledgeListView_, knowledge.GetRealAgent().GetName().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnKnowledgeUpdated
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnKnowledgeUpdated( MOS_Gtia& /*gtia*/, MOS_AgentKnowledge& knowledge )
{
    if( &knowledge != pSelectedKnowledge_ )
        return;
    this->UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnKnowledgeDeleted
/** @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge )
{
    if( &gtia != selectedItem_.pGtia_ )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_AgentKnowledge*>*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::OnContextMenuRequested
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem<MOS_AgentKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_AgentKnowledge*>*)pItem;
    MOS_SelectedElement selectedElement( *(pCastItem->GetValue()) );
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::ToggleDisplayOwnTeam
// Created: APE 2004-05-27
// -----------------------------------------------------------------------------
void MOS_AgentKnowledgePanel::ToggleDisplayOwnTeam()
{
    this->UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentKnowledgePanel::GetSelectedGtia
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
MOS_Gtia* MOS_AgentKnowledgePanel::GetSelectedGtia( const MOS_SelectedElement& item )
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetGtia();
    else if( selectedItem_.pGtia_ )
        return selectedItem_.pGtia_;
    else if( selectedItem_.pAgentKnowledge_ )
        return & selectedItem_.pAgentKnowledge_->GetOwner();
    return 0;
}
