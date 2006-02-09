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
#include "MOS_Display.h"
#include "MOS_DisplayGroup.h"
#include "MOS_DisplayItem.h"

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

    display_ = new MOS_Display( this );
    display_->AddGroup( "Détails" )
                .AddItem( "Id:" )
                .AddItem( "Objet associé:" )
                .AddItem( "Position:" )
                .AddItem( "Type:" )
                .AddItem( "Construction:" )
                .AddItem( "Valeur:" )
                .AddItem( "Contournement:" )
                .AddItem( "En préparation:" )
                .AddItem( "Perçu:" )
                .AddItem( "Pertinence:" );
    display_->AddGroup( "Site de franchissement" )
                .AddItem( "Largeur:" )
                .AddItem( "Profondeur:" )
                .AddItem( "Vitesse courant:" )
                .AddItem( "Berges à aménager:" );
    display_->AddGroup( "Nuage/zone NBC" )
                .AddItem( "Agent NBC:" );
    display_->AddGroup( "ROTA" )
                .AddItem( "Danger:" )
                .AddItem( "Agents NBC:" );
    display_->AddGroup( "Camp de prisonniers/réfugiés" )
                .AddItem( "TC2:" );
    display_->AddGroup( "Itinéraire logistique" )
                .AddItem( "Largeur:" )
                .AddItem( "Longueur:" )
                .AddItem( "Débit:" )
                .AddItem( "Poids maximum:" )
                .AddItem( "Equipé:" );

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
    // NOTHING
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

namespace
{
    template< typename T >
    QString IfSet( const T& value, const QString& message )
    {
        return value.IsSet() ? message : "";
    }
    template< typename T >
    QString IfSet( const T& value, const std::string& message )
    {
        return value.IsSet() ? message.c_str() : "";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledgePanel::UpdateSelected
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void MOS_ObjectKnowledgePanel::UpdateSelected()
{
    display_->Clear();
    
    display_->Group( "Site de franchissement" ).hide();
    display_->Group( "Nuage/zone NBC" ).hide();
    display_->Group( "ROTA" ).hide();
    display_->Group( "Camp de prisonniers/réfugiés"  ).hide();
    display_->Group( "Itinéraire logistique" ).hide();

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    MOS_ObjectKnowledge& k = *pSelectedKnowledge_;

    display_->Group( "Détails" )
                .Display( "Id:", MOS_Display::Id( k.nID_) )
                .Display( "Objet associé:", ( k.pRealObject_.IsSet() && k.pRealObject_ != 0 ) ? MOS_Display::Id( k.pRealObject_->GetID() ) : "" )
                .Display( "Position:", IfSet( k.strPos_, k.strPos_ ) )
                .Display( "Type:", ENT_Tr::ConvertFromObjectType( (E_ObjectType)k.nObjectTypeID_ ) )
                .Display( "Construction:", IfSet( k.nPourcentageConstruction_, QString::number( k.nPourcentageConstruction_ ) + "%" ) )
                .Display( "Valeur:", IfSet( k.nPourcentageValorisation_, QString::number( k.nPourcentageValorisation_ ) + "%" ) )
                .Display( "Contournement:", IfSet( k.nPourcentageContournement_, QString::number( k.nPourcentageContournement_ ) + "%" ) )
                .Display( "En préparation:", IfSet( k.bEnPreparation_, MOS_Display::YesNo( k.bEnPreparation_ ) ) )
                .Display( "Perçu:", IfSet( k.bIsPerceived_, MOS_Display::YesNo( k.bIsPerceived_ ) ) )
                .Display( "Pertinence:", IfSet( k.nRelevance_, QString::number( k.nRelevance_ ) ) );

    if( k.automatePerceptionSet_.IsSet() )
    {
        for( MOS_ObjectKnowledge::IT_AutomatePerceptionSet it = k.automatePerceptionSet_.Data().begin(); it != k.automatePerceptionSet_.Data().end(); ++it )
            new QListViewItem( pPerceptionListView_, (*it)->GetName().c_str() );
    }

    if( k.nSiteFranchissementLargeur_.IsSet() )
        display_->Group( "Site de franchissement" )
                    .Display( "Largeur:", k.nSiteFranchissementLargeur_ )
                    .Display( "Profondeur:", k.nSiteFranchissementProfondeur_ )
                    .Display( "Vitesse courant:", k.nSiteFranchissementVitesseCourant_ )
                    .Display( "Berges à aménager:", MOS_Display::YesNo( k.nSiteFranchissementBergesAAmenager_ ) );

    if( k.nNuageNBCAgentNbcID_.IsSet() )
        display_->Group( "Nuage/zone NBC" ).Display( "Agent NBC:", MOS_App::GetApp().GetNBCName( k.nNuageNBCAgentNbcID_ ) );

    if( k.nROTADanger_.IsSet() )
    {
        const std::vector< uint >& nbcAgents = k.rotaNBCAgents_.Data();
        std::stringstream ss;
        for( uint i = 0; i < nbcAgents.size(); ++i ) {
            if( i > 0 )
                ss << ", ";
            ss << MOS_App::GetApp().GetNBCName( nbcAgents[ i ] );
        }

        display_->Group( "ROTA" )
                    .Display( "Danger:", k.nROTADanger_ )
                    .Display( "Agents NBC:", ss.str() );
    }

    if( k.nCampTC2ID_.IsSet() )
        display_->Group( "Camp de prisonniers/réfugiés" )
                    .Display( "TC2:", k.nCampTC2ID_ );

    if( k.nLogRouteFlow_.IsSet() ) 
        display_->Group( "Itinéraire logistique" )
                    .Display( "Largeur:",       k.nLogRouteWidth_ )
                    .Display( "Longueur:",      k.nLogRouteLength_ )
                    .Display( "Débit:",         k.nLogRouteFlow_ )
                    .Display( "Poids maximum:", k.nLogRouteMaxWeight_ )
                    .Display( "Equipé:",        MOS_Display::YesNo( k.bLogRouteEquipped_ ) );
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
    if( pCastItem && pCastItem->GetValue()->strPos_.IsSet() )
        emit CenterOnPoint( pCastItem->GetValue()->vCenter_ );
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
