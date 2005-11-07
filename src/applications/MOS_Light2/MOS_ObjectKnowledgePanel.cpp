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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:34 $
// $Revision: 9 $
// $Workfile: MOS_ObjectKnowledgePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ObjectKnowledgePanel.h"
#include "moc_MOS_ObjectKnowledgePanel.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"
#include "MOS_Tools.h"
#include "MOS_Object_ABC.h"
#include "MOS_Agent.h"
#include "MOS_Team.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_ActionContext.h"
#include "MOS_SelectedElement.h"
#include "MOS_Gtia.h"
#include "MOS_AgentKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel constructor
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
MOS_ObjectKnowledgePanel::MOS_ObjectKnowledgePanel( QWidget* pParent )
    : MOS_InfoPanel_ABC     ( pParent )
    , pSelectedKnowledge_   ( 0 )
    , pPopupMenu_           ( new QPopupMenu( this ) )
    , pTeam_( 0 )
{
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

    //! @name crossing
    //@{
    pCrossingGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Site de franchissement" ), this );
    new QLabel( tr( "Largeur:" ), pCrossingGroup_ );
    pCrossingWidthLabel_ = new QLabel( pCrossingGroup_ );
    new QLabel( tr( "Profondeur:" ), pCrossingGroup_ );
    pCrossingDepthLabel_ = new QLabel( pCrossingGroup_ );
    new QLabel( tr( "Vitesse courant:" ), pCrossingGroup_ );
    pCrossingRiverSpeedLabel_ = new QLabel( pCrossingGroup_ );
    new QLabel( tr( "Berges à aménager:" ), pCrossingGroup_ );
    pCrossingBanksNeedWorkLabel_ = new QLabel( pCrossingGroup_ );
    //@}
    
    //! @name NBC
    //@{
    pNBCGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Nuage/zone NBC" ), this );
    new QLabel( tr( "Agent NBC:" ), pNBCGroup_ );
    pNBCAgentLabel_ = new QLabel( pNBCGroup_ );
    //@}

    //! @name ROTA
    //@{
    pROTAGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "ROTA" ), this );
    new QLabel( tr( "Danger:" ), pROTAGroup_ );
    pROTADangerLabel_ = new QLabel( pROTAGroup_ );
    new QLabel( tr( "Agents NBC:" ), pROTAGroup_ );
    pROTANBCAgentsLabel_ = new QLabel( pROTAGroup_ );
    //@}

    //! @name prisoner/refugee camp
    //@{
    pCampGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Camp de prisonniers/réfugiés" ), this );
    new QLabel( tr( "TC2:" ), pCampGroup_ );
    pCampTC2ID_ = new QLabel( pCampGroup_ );
    //@}

    //! @name logistic route
    //@{
    pLogRouteGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Itinéraire logistique" ), this );
    new QLabel( tr( "Largeur:" ), pLogRouteGroup_ );
    pLogRouteWidth_ = new QLabel( pLogRouteGroup_ );
    new QLabel( tr( "Longueur:" ), pLogRouteGroup_ );
    pLogRouteLength_ = new QLabel( pLogRouteGroup_ );
    new QLabel( tr( "Débit:" ), pLogRouteGroup_ );
    pLogRouteFlow_ = new QLabel( pLogRouteGroup_ );
    new QLabel( tr( "Poids maximum:" ), pLogRouteGroup_ );
    pLogRouteMaxWeight_ = new QLabel( pLogRouteGroup_ );
    new QLabel( tr( "Equipé:" ), pLogRouteGroup_ );
    pLogRouteEquipped_ = new QLabel( pLogRouteGroup_ );
    //@}

    new QLabel( tr( "Perçu:" ), pDetails );
    pIsPreceivedLabel_ = new QLabel( pDetails );

    new QLabel( tr( "Pertinence:" ), pDetails );
    pRevelanceLabel_ = new QLabel( pDetails );

    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );

    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeCreated( MOS_Team&, MOS_ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeCreated( MOS_Team&, MOS_ObjectKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeUpdated( MOS_Team&, MOS_ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeUpdated( MOS_Team&, MOS_ObjectKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ) );

    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel destructor
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
MOS_ObjectKnowledgePanel::~MOS_ObjectKnowledgePanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnUpdate()
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
// Name: MOS_ObjectKnowledgePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnClearSelection()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::UpdateList
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::UpdateList()
{
    MOS_Team* pTeam = GetSelectedTeam( selectedItem_ );
    MOS_ObjectKnowledge* pKnowledge = selectedItem_.pObjectKnowledge_;

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
                new MT_ValuedListViewItem<MOS_ObjectKnowledge*>( it->second, pKnowledgeListView_,
                QString( ENT_Tr::ConvertFromObjectType( (E_ObjectType)it->second->nObjectTypeID_ ).c_str() )
                + " [" + QString::number( it->second->GetID() ) + "]" );
        }
        pTeam_ = pTeam;
    }

    // Try to select the appropriate knowledge.
    if( pKnowledge != 0 )
    {
        QListViewItem* pItem = pKnowledgeListView_->firstChild();
        while( pItem != 0 )
        {
            MT_ValuedListViewItem<MOS_ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_ObjectKnowledge*>*)pItem;
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
// Name: MOS_ObjectKnowledgePanel::UpdateSelected
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::UpdateSelected()
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

    pCrossingGroup_->hide();
    pNBCGroup_->hide();
    pROTAGroup_->hide();
    pCampGroup_->hide();
    pLogRouteGroup_->hide();

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    pIdLabel_->setText( QString::number( pSelectedKnowledge_->GetID() ) );
    pObjectTypeLabel_->setText( ENT_Tr::ConvertFromObjectType( (E_ObjectType)pSelectedKnowledge_->nObjectTypeID_ ).c_str() );
    
    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_RealObject ) && pSelectedKnowledge_->GetRealObject() != 0 )
        pAssociatedObjectLabel_->setText( QString::number( pSelectedKnowledge_->GetRealObject()->GetID() ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_Localisation ) )
    {
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pSelectedKnowledge_->GetCenter(), strPos );
        pPositionLabel_->setText( strPos.c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageConstruction ) )
        pPercentBuiltLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageConstruction_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageValorisation ) )
        pPercentValueLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageValorisation_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_PourcentageContournement ) )
        pPercentAroundLabel_->setText( QString::number( pSelectedKnowledge_->nPourcentageContournement_ ) + QString( "%" ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_EnPreparation ) )
        pIsUnderPrepLabel_->setText( pSelectedKnowledge_->bEnPreparation_ ? tr( "Oui" ) : tr( "Non" ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_IsPerceived ) )
        pIsPreceivedLabel_->setText( pSelectedKnowledge_->bIsPerceived_ ? tr( "Oui" ) : tr( "Non" ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_Relevance ) )
        pRevelanceLabel_->setText( QString::number( pSelectedKnowledge_->nRelevance_ ) );

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_AutomatePerception ) )
    {
        for( MOS_ObjectKnowledge::IT_AutomatePerceptionSet it = pSelectedKnowledge_->automatePerceptionSet_.begin(); it != pSelectedKnowledge_->automatePerceptionSet_.end(); ++it )
            new QListViewItem( pPerceptionListView_, (*it)->GetName().c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_RiverCrossing ) )
    {
        pCrossingGroup_->show();
        pCrossingWidthLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementLargeur() ) );
        pCrossingDepthLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementProfondeur() ) );
        pCrossingRiverSpeedLabel_->setText( QString::number( pSelectedKnowledge_->GetSiteFranchissementVitesseCourant() ) );
        pCrossingBanksNeedWorkLabel_->setText( pSelectedKnowledge_->GetSiteFranchissementBergesAAmenager() ? tr( "Oui" ) : tr( "Non" ) );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_NBCCloud ) )
    {
        pNBCGroup_->show();
        pNBCAgentLabel_->setText( (MOS_App::GetApp().GetNBCName( pSelectedKnowledge_->GetNuageNBCAgentNbcId() )).c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_ROTA ) )
    {
        pROTAGroup_->show();
        pROTADangerLabel_->setText( QString::number( pSelectedKnowledge_->GetROTADanger() ) );
        const std::vector< uint >& nbcAgents = pSelectedKnowledge_->GetROTANBCAgents();
        std::stringstream ss;
        for( uint i = 0; i < nbcAgents.size(); ++i )
        {
            if( i > 0 )
                ss << ", ";
            ss << MOS_App::GetApp().GetNBCName( nbcAgents[ i ] );
        }
        pROTANBCAgentsLabel_->setText( ss.str().c_str() );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_Camp ) )
    {
        pCampGroup_->show();
        pCampTC2ID_->setText( QString::number( pSelectedKnowledge_->GetCampTC2ID() ) );
    }

    if( pSelectedKnowledge_->IsValid( MOS_ObjectKnowledge::eUpdated_LogisticRoute ) )
    {
        pLogRouteGroup_->show();
        pLogRouteWidth_->setText( QString::number( pSelectedKnowledge_->GetLogRouteWidth() ) );
        pLogRouteLength_->setText( QString::number( pSelectedKnowledge_->GetLogRouteLength() ) );
        pLogRouteFlow_->setText( QString::number( pSelectedKnowledge_->GetLogRouteFlow() ) );
        pLogRouteMaxWeight_->setText( QString::number( pSelectedKnowledge_->GetLogRouteMaxWeight() ) );
        pLogRouteEquipped_->setText( pSelectedKnowledge_->GetLogRouteEquipped() ? tr( "oui" ) : tr( "non" ) );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnSelectionChanged
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_ObjectKnowledge*>*)pItem;
        pSelectedKnowledge_ = pCastItem->GetValue();
        MOS_SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
        pSelectedKnowledge_ = 0;

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnRequestCenter
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;

    MT_ValuedListViewItem<MOS_ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_ObjectKnowledge*>*)pItem;
    if( pCastItem->GetValue()->nAttrUpdated_ & MOS_ObjectKnowledge::eUpdated_Localisation )
        emit CenterOnPoint( pCastItem->GetValue()->GetCenter() );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnKnowledgeCreated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnKnowledgeCreated( MOS_Team& team, MOS_ObjectKnowledge& knowledge )
{
    if( &team != pTeam_ )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || selectedItem_.pTeam_->GetID() != knowledge.GetOwner().GetID() )
        new MT_ValuedListViewItem<MOS_ObjectKnowledge*>( &knowledge, pKnowledgeListView_, 
        QString( ENT_Tr::ConvertFromObjectType( (E_ObjectType)knowledge.nObjectTypeID_ ).c_str() )
        + " [" + QString::number( knowledge.GetID() ) + "]" );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnKnowledgeUpdated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnKnowledgeUpdated( MOS_Team& /*team*/, MOS_ObjectKnowledge& knowledge )
{
    if( &knowledge == pSelectedKnowledge_ )
        UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnKnowledgeDeleted
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge )
{
    if( &team != pTeam_ )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem<MOS_ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_ObjectKnowledge*>*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::OnContextMenuRequested
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem<MOS_ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<MOS_ObjectKnowledge*>*)pItem;
    MOS_SelectedElement selectedElement( *(pCastItem->GetValue()) );
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::ToggleDisplayOwnTeam
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::ToggleDisplayOwnTeam()
{
    this->UpdateList();
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::GetSelectedTeam
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
MOS_Team* MOS_ObjectKnowledgePanel::GetSelectedTeam( const MOS_SelectedElement& /*item*/ )
{
    if( selectedItem_.pAgent_ )
        return & selectedItem_.pAgent_->GetTeam();
    else if( selectedItem_.pGtia_ )
        return & selectedItem_.pGtia_->GetTeam();
    else if( selectedItem_.pTeam_ )
        return selectedItem_.pTeam_;
    else if( selectedItem_.pAgentKnowledge_ )
        return & selectedItem_.pAgentKnowledge_->GetOwner().GetTeam();
    else if( selectedItem_.pObjectKnowledge_ )
        return &selectedItem_.pObjectKnowledge_->GetOwner();
    return 0;
}
