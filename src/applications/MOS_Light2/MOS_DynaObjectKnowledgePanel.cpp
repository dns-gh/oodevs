// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:34 $
// $Revision: 9 $
// $Workfile: MOS_DynaObjectKnowledgePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_DynaObjectKnowledgePanel.h"
#include "moc_MOS_DynaObjectKnowledgePanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"
#include "MOS_Tools.h"
#include "MOS_DynaObject.h"
#include "MOS_Agent.h"
#include "MOS_Team.h"
#include "MOS_DynaObjectKnowledge.h"
#include "MOS_ActionContext.h"
#include "MOS_SelectedElement.h"
#include "MOS_Gtia.h"
#include "MOS_AgentKnowledge.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObjectKnowledgePanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel constructor
/** @param  pParent 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
MOS_DynaObjectKnowledgePanel::MOS_DynaObjectKnowledgePanel( QWidget* pParent )
    : MOS_InfoPanel_ABC     ( pParent )
    , pSelectedKnowledge_   ( 0 )
    , pPopupMenu_           ( new QPopupMenu( this ) )
    , pTeam_( 0 )
{
    this->setMargin( 5 );
    this->setSpacing( 5 );

    pKnowledgeListView_ = new QListView( this );
    pKnowledgeListView_->addColumn( tr( "Objets connus" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    QGroupBox* pDetails = new QGroupBox( 2, Qt::Horizontal, tr( "Détails" ), this );

    new QLabel( tr( "Id:" ), pDetails );
    pIdLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Objet associé:" ), pDetails );
    pAssociatedObjectLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Position:" ), pDetails );
    pPositionLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Type:" ), pDetails );
    pObjectTypeLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Construction:" ), pDetails );
    pPercentBuiltLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Valeur:" ), pDetails );
    pPercentValueLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Contournement:" ), pDetails );
    pPercentAroundLabel_ = new QLabel( pDetails );

    new QLabel( tr( "En préparation:" ), pDetails );
    pIsUnderPrepLabel_ = new QLabel( pDetails );

    pCrossingWidthTitle_ = new QLabel( tr( "Largeur:" ), this );
    pCrossingWidthLabel_ = new QLabel( this );
    pCrossingWidthTitle_->hide();
    pCrossingWidthLabel_->hide();

    pCrossingDepthTitle_ = new QLabel( tr( "Profondeur:" ), this );
    pCrossingDepthLabel_ = new QLabel( this );
    pCrossingDepthTitle_->hide();
    pCrossingDepthLabel_->hide();

    pCrossingRiverSpeedTitle_ = new QLabel( tr( "Vitesse courant:" ), this );
    pCrossingRiverSpeedLabel_ = new QLabel( this );
    pCrossingRiverSpeedTitle_->hide();
    pCrossingRiverSpeedLabel_->hide();

    pCrossingBanksNeedWorkTitle_ = new QLabel( tr( "Berges à aménager:" ), this );
    pCrossingBanksNeedWorkLabel_ = new QLabel( this );
    pCrossingBanksNeedWorkTitle_->hide();
    pCrossingBanksNeedWorkLabel_->hide();
    
    pNBCAgentTitle_ = new QLabel( tr( "Nuage Agent NBC:" ), this );
    pNBCAgentLabel_ = new QLabel( this );
    pNBCAgentTitle_->hide();
    pNBCAgentLabel_->hide();

    new QLabel( tr( "Perçu:" ), pDetails );
    pIsPreceivedLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Pertinence:" ), pDetails );
    pRevelanceLabel_ = new QLabel( pDetails );

    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );

    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeCreated( MOS_Team&, MOS_DynaObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeCreated( MOS_Team&, MOS_DynaObjectKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeUpdated( MOS_Team&, MOS_DynaObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeUpdated( MOS_Team&, MOS_DynaObjectKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_DynaObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeDeleted( MOS_Team&, MOS_DynaObjectKnowledge& ) ) );

    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel destructor
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
MOS_DynaObjectKnowledgePanel::~MOS_DynaObjectKnowledgePanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnUpdate()
{
    if( GetSelectedTeam( selectedItem_ ) == 0 )
    {
        pTeam_ = 0;
        pKnowledgeListView_->clear();
        pSelectedKnowledge_ = 0;
        UpdateSelected();
    }
    else
        UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::UpdateList
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::UpdateList()
{
    MOS_Team* pTeam = GetSelectedTeam( selectedItem_ );
    MOS_DynaObjectKnowledge* pKnowledge = selectedItem_.pDynaObjectKnowledge_;

    if( pTeam == 0 )
        return;
    
    if( pTeam_ != pTeam )
    {
        pKnowledgeListView_->clear();
        const MOS_Team::T_ObjectKnowledgeMap& knowledges = pTeam->GetObjectKnowledges();
        for( MOS_Team::CIT_ObjectKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pTeam->GetID() != (*it).second->GetOwner().GetID() )
                new MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>( (*it).second, pKnowledgeListView_, QString::number( (*it).second->GetID() ) );
        }
        pTeam_ = pTeam;
    }

    // Try to select the appropriate knowledge.
    if( pKnowledge != 0 )
    {
        QListViewItem* pItem = pKnowledgeListView_->firstChild();
        while( pItem != 0 )
        {
            MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>*)pItem;
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
// Name: MOS_DynaObjectKnowledgePanel::UpdateSelected
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::UpdateSelected()
{
    pIdLabel_               ->setText( "-" );
    pAssociatedObjectLabel_ ->setText( "-" );
    pPositionLabel_         ->setText( "-" );
    pObjectTypeLabel_       ->setText( "-" );
    pPercentBuiltLabel_     ->setText( "-" );
    pPercentValueLabel_     ->setText( "-" );
    pPercentAroundLabel_    ->setText( "-" );
    pIsUnderPrepLabel_      ->setText( "-" );
    pIsPreceivedLabel_      ->setText( "-" );
    pRevelanceLabel_        ->setText( "-" );

    pCrossingWidthTitle_->hide();
    pCrossingWidthLabel_->hide();
    pCrossingDepthTitle_->hide();
    pCrossingDepthLabel_->hide();
    pCrossingRiverSpeedTitle_->hide();
    pCrossingRiverSpeedLabel_->hide();
    pCrossingBanksNeedWorkTitle_->hide();
    pCrossingBanksNeedWorkLabel_->hide();
    pNBCAgentTitle_->hide();
    pNBCAgentLabel_->hide();

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    pIdLabel_->setText( QString::number( pSelectedKnowledge_->GetID() ) );
    pObjectTypeLabel_->setText( ENT_Tr::ConvertFromObjectType( (E_ObjectType)pSelectedKnowledge_->nObjectTypeID_ ).c_str() );
    
    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_RealObject ) && pSelectedKnowledge_->GetRealObject() != 0 )
        pAssociatedObjectLabel_->setText( QString::number( pSelectedKnowledge_->GetRealObject()->GetID() ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_Localisation ) )
    {
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pSelectedKnowledge_->GetCenter(), strPos );
        pPositionLabel_->setText( strPos.c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_PourcentageConstruction ) )
        pPercentBuiltLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageConstruction_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_PourcentageValorisation ) )
        pPercentValueLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageValorisation_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_PourcentageContournement ) )
        pPercentAroundLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageContournement_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_EnPreparation ) )
        pIsUnderPrepLabel_->setText( pSelectedKnowledge_->bEnPreparation_ ? tr( "Oui" ) : tr( "Non" ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_IsPerceived ) )
        pIsPreceivedLabel_->setText( pSelectedKnowledge_->bIsPerceived_ ? tr( "Oui" ) : tr( "Non" ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_Relevance ) )
        pRevelanceLabel_->setText( QString::number( pSelectedKnowledge_->nRelevance_ ) );

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_AutomatePerception ) )
    {
        for( MOS_DynaObjectKnowledge::IT_AutomatePerceptionSet it = pSelectedKnowledge_->automatePerceptionSet_.begin(); it != pSelectedKnowledge_->automatePerceptionSet_.end(); ++it )
            new QListViewItem( pPerceptionListView_, (*it)->GetName().c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_RiverCrossing ) )
    {
        pCrossingWidthTitle_->show();
        pCrossingWidthLabel_->show();
        pCrossingWidthLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementLargeur() ) );

        pCrossingDepthTitle_->show();
        pCrossingDepthLabel_->show();
        pCrossingDepthLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementProfondeur() ) );

        pCrossingRiverSpeedTitle_->show();
        pCrossingRiverSpeedLabel_->show();
        pCrossingRiverSpeedLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementVitesseCourant() ) );

        pCrossingBanksNeedWorkTitle_->show();
        pCrossingBanksNeedWorkLabel_->show();
        pCrossingBanksNeedWorkLabel_->setText( pSelectedKnowledge_->GetSiteFranchissementBergesAAmenager() ? tr( "Oui" ) : tr( "Non" ) );
    }

    if( pSelectedKnowledge_->IsValid( MOS_DynaObjectKnowledge::eUpdated_NBCCloud ) )
    {
        pNBCAgentTitle_->show();
        pNBCAgentLabel_->show();

        pNBCAgentLabel_->setText( (MOS_App::GetApp().GetNBCName( pSelectedKnowledge_->GetNuageNBCAgentNbcId() )).c_str() );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnSelectionChanged
/** @param  pItem 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>*)pItem;
        pSelectedKnowledge_ = pCastItem->GetValue();
        MOS_SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
        pSelectedKnowledge_ = 0;

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnRequestCenter
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;

    MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>*)pItem;
    if( pCastItem->GetValue()->nAttrUpdated_ & MOS_DynaObjectKnowledge::eUpdated_Localisation )
        emit CenterOnPoint( pCastItem->GetValue()->GetCenter() );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnKnowledgeCreated
/** @param  team 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnKnowledgeCreated( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge )
{
    if( &team != selectedItem_.pTeam_ )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || selectedItem_.pTeam_->GetID() != knowledge.GetOwner().GetID() )
        new MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>( &knowledge, pKnowledgeListView_, QString::number( knowledge.GetID() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnKnowledgeUpdated
/** @param  team 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnKnowledgeUpdated( MOS_Team& /*team*/, MOS_DynaObjectKnowledge& knowledge )
{
    if( &knowledge == pSelectedKnowledge_ )
        UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnKnowledgeDeleted
/** @param  team 
    @param  knowledge 
*/
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnKnowledgeDeleted( MOS_Team& team, MOS_DynaObjectKnowledge& knowledge )
{
    if( &team != selectedItem_.pTeam_ )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::OnContextMenuRequested
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_DynaObjectKnowledge*>*)pItem;
    MOS_SelectedElement selectedElement( *(pCastItem->GetValue()) );
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::ToggleDisplayOwnTeam
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_DynaObjectKnowledgePanel::ToggleDisplayOwnTeam()
{
    this->UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledgePanel::GetSelectedTeam
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
MOS_Team* MOS_DynaObjectKnowledgePanel::GetSelectedTeam( const MOS_SelectedElement& item )
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetTeam();
    else if( selectedItem_.pGtia_ )
        return & selectedItem_.pGtia_->GetTeam();
    else if( selectedItem_.pTeam_ )
        return selectedItem_.pTeam_;
    else if( selectedItem_.pAgentKnowledge_ )
        return & selectedItem_.pAgentKnowledge_->GetOwner().GetTeam();
    else if( selectedItem_.pDynaObjectKnowledge_ )
        return &selectedItem_.pDynaObjectKnowledge_->GetOwner();
    return 0;
}
