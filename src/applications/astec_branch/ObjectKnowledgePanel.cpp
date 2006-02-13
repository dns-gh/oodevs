// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:34 $
// $Revision: 9 $
// $Workfile: ObjectKnowledgePanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ObjectKnowledgePanel.h"
#include "moc_ObjectKnowledgePanel.cpp"

#include "App.h"
#include "MainWindow.h"
#include "World.h"
#include "Tools.h"
#include "Object_ABC.h"
#include "Agent.h"
#include "Team.h"
#include "ObjectKnowledge.h"
#include "ActionContext.h"
#include "SelectedElement.h"
#include "Gtia.h"
#include "AgentKnowledge.h"
#include "Display.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel constructor
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::ObjectKnowledgePanel( QWidget* pParent )
    : InfoPanel_ABC     ( pParent )
    , pSelectedKnowledge_   ( 0 )
    , pPopupMenu_           ( new QPopupMenu( this ) )
    , pTeam_( 0 )
{
    pKnowledgeListView_ = new QListView( this );
    pKnowledgeListView_->addColumn( tr( "Objets connus" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
    pOwnTeamCheckBox_->setChecked( true );

    display_ = new Display( this );
    display_->AddGroup( "D�tails" )
                .AddItem( "Id:" )
                .AddItem( "Objet associ�:" )
                .AddItem( "Position:" )
                .AddItem( "Type:" )
                .AddItem( "Construction:" )
                .AddItem( "Valeur:" )
                .AddItem( "Contournement:" )
                .AddItem( "En pr�paration:" )
                .AddItem( "Per�u:" )
                .AddItem( "Pertinence:" );
    display_->AddGroup( "Site de franchissement" )
                .AddItem( "Largeur:" )
                .AddItem( "Profondeur:" )
                .AddItem( "Vitesse courant:" )
                .AddItem( "Berges � am�nager:" );
    display_->AddGroup( "Nuage/zone NBC" )
                .AddItem( "Agent NBC:" );
    display_->AddGroup( "ROTA" )
                .AddItem( "Danger:" )
                .AddItem( "Agents NBC:" );
    display_->AddGroup( "Camp de prisonniers/r�fugi�s" )
                .AddItem( "TC2:" );
    display_->AddGroup( "Itin�raire logistique" )
                .AddItem( "Largeur:" )
                .AddItem( "Longueur:" )
                .AddItem( "D�bit:" )
                .AddItem( "Poids maximum:" )
                .AddItem( "Equip�:" );

    pPerceptionListView_ = new QListView( this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );

    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeCreated( Team&, ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeCreated( Team&, ObjectKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeUpdated( Team&, ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeUpdated( Team&, ObjectKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ),
             this,                 SLOT(     OnKnowledgeDeleted( Team&, ObjectKnowledge& ) ) );

    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),               &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel destructor
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
ObjectKnowledgePanel::~ObjectKnowledgePanel()
{
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnUpdate()
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
// Name: ObjectKnowledgePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnClearSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::UpdateList
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::UpdateList()
{
    Team* pTeam = GetSelectedTeam( selectedItem_ );
    ObjectKnowledge* pKnowledge = selectedItem_.pObjectKnowledge_;

    if( pTeam == 0 )
        return;
    
    if( pTeam_ != pTeam )
    {
        pKnowledgeListView_->clear();
        const Team::T_ObjectKnowledgeMap& knowledges = pTeam->GetObjectKnowledges();
        for( Team::CIT_ObjectKnowledgeMap it = knowledges.begin(); it != knowledges.end(); ++it )
        {
            // We only display knowledges concerning our own team if the appropriate check box is checked.
            if( pOwnTeamCheckBox_->isChecked() || pTeam->GetID() != (*it).second->GetOwner().GetID() )
                new MT_ValuedListViewItem<ObjectKnowledge*>( it->second, pKnowledgeListView_,
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
            MT_ValuedListViewItem<ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<ObjectKnowledge*>*)pItem;
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
// Name: ObjectKnowledgePanel::UpdateSelected
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::UpdateSelected()
{
    display_->Clear();
    
    display_->Group( "Site de franchissement" ).hide();
    display_->Group( "Nuage/zone NBC" ).hide();
    display_->Group( "ROTA" ).hide();
    display_->Group( "Camp de prisonniers/r�fugi�s"  ).hide();
    display_->Group( "Itin�raire logistique" ).hide();

    pPerceptionListView_->clear();

    if( pSelectedKnowledge_ == 0 )
        return;

    ObjectKnowledge& k = *pSelectedKnowledge_;

    display_->Group( "D�tails" )
                .Display( "Id:", Display::Id( k.nID_) )
                .Display( "Objet associ�:", ( k.pRealObject_.IsSet() && k.pRealObject_ != 0 ) ? Display::Id( k.pRealObject_->GetID() ) : "" )
                .Display( "Position:", IfSet( k.strPos_, k.strPos_ ) )
                .Display( "Type:", ENT_Tr::ConvertFromObjectType( (E_ObjectType)k.nObjectTypeID_ ) )
                .Display( "Construction:", IfSet( k.nPourcentageConstruction_, QString::number( k.nPourcentageConstruction_ ) + "%" ) )
                .Display( "Valeur:", IfSet( k.nPourcentageValorisation_, QString::number( k.nPourcentageValorisation_ ) + "%" ) )
                .Display( "Contournement:", IfSet( k.nPourcentageContournement_, QString::number( k.nPourcentageContournement_ ) + "%" ) )
                .Display( "En pr�paration:", IfSet( k.bEnPreparation_, Display::YesNo( k.bEnPreparation_ ) ) )
                .Display( "Per�u:", IfSet( k.bIsPerceived_, Display::YesNo( k.bIsPerceived_ ) ) )
                .Display( "Pertinence:", IfSet( k.nRelevance_, QString::number( k.nRelevance_ ) ) );

    if( k.automatePerceptionSet_.IsSet() )
    {
        for( ObjectKnowledge::IT_AutomatePerceptionSet it = k.automatePerceptionSet_.Data().begin(); it != k.automatePerceptionSet_.Data().end(); ++it )
            new QListViewItem( pPerceptionListView_, (*it)->GetName().c_str() );
    }

    if( k.nSiteFranchissementLargeur_.IsSet() )
        display_->Group( "Site de franchissement" )
                    .Display( "Largeur:", k.nSiteFranchissementLargeur_ )
                    .Display( "Profondeur:", k.nSiteFranchissementProfondeur_ )
                    .Display( "Vitesse courant:", k.nSiteFranchissementVitesseCourant_ )
                    .Display( "Berges � am�nager:", Display::YesNo( k.nSiteFranchissementBergesAAmenager_ ) );

    if( k.nNuageNBCAgentNbcID_.IsSet() )
        display_->Group( "Nuage/zone NBC" ).Display( "Agent NBC:", App::GetApp().GetNBCName( k.nNuageNBCAgentNbcID_ ) );

    if( k.nROTADanger_.IsSet() )
    {
        const std::vector< uint >& nbcAgents = k.rotaNBCAgents_.Data();
        std::stringstream ss;
        for( uint i = 0; i < nbcAgents.size(); ++i ) {
            if( i > 0 )
                ss << ", ";
            ss << App::GetApp().GetNBCName( nbcAgents[ i ] );
        }

        display_->Group( "ROTA" )
                    .Display( "Danger:", k.nROTADanger_ )
                    .Display( "Agents NBC:", ss.str() );
    }

    if( k.nCampTC2ID_.IsSet() )
        display_->Group( "Camp de prisonniers/r�fugi�s" )
                    .Display( "TC2:", k.nCampTC2ID_ );

    if( k.nLogRouteFlow_.IsSet() ) 
        display_->Group( "Itin�raire logistique" )
                    .Display( "Largeur:",       k.nLogRouteWidth_ )
                    .Display( "Longueur:",      k.nLogRouteLength_ )
                    .Display( "D�bit:",         k.nLogRouteFlow_ )
                    .Display( "Poids maximum:", k.nLogRouteMaxWeight_ )
                    .Display( "Equip�:",        Display::YesNo( k.bLogRouteEquipped_ ) );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnSelectionChanged
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnSelectionChanged( QListViewItem* pItem )
{
    if( pItem != 0 )
    {
        MT_ValuedListViewItem<ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<ObjectKnowledge*>*)pItem;
        pSelectedKnowledge_ = pCastItem->GetValue();
        SelectedElement selectedElement( *pSelectedKnowledge_ );
        emit ElementSelected( selectedElement );
    }
    else
        pSelectedKnowledge_ = 0;

    UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnRequestCenter
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnRequestCenter()
{
    QListViewItem* pItem = pKnowledgeListView_->selectedItem();
    if( pItem == 0 )
        return;

    MT_ValuedListViewItem<ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<ObjectKnowledge*>*)pItem;
    if( pCastItem && pCastItem->GetValue()->strPos_.IsSet() )
        emit CenterOnPoint( pCastItem->GetValue()->vCenter_ );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnKnowledgeCreated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnKnowledgeCreated( Team& team, ObjectKnowledge& knowledge )
{
    if( &team != pTeam_ )
        return;

    // We only display knowledges concerning our own team if the appropriate check box is checked.
    if( pOwnTeamCheckBox_->isChecked() || selectedItem_.pTeam_->GetID() != knowledge.GetOwner().GetID() )
        new MT_ValuedListViewItem<ObjectKnowledge*>( &knowledge, pKnowledgeListView_, 
        QString( ENT_Tr::ConvertFromObjectType( (E_ObjectType)knowledge.nObjectTypeID_ ).c_str() )
        + " [" + QString::number( knowledge.GetID() ) + "]" );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnKnowledgeUpdated
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnKnowledgeUpdated( Team& /*team*/, ObjectKnowledge& knowledge )
{
    if( &knowledge == pSelectedKnowledge_ )
        UpdateSelected();
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnKnowledgeDeleted
// Created: APE 2004-05-04
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge )
{
    if( &team != pTeam_ )
        return;

    QListViewItem* pItem = pKnowledgeListView_->firstChild();
    while( pItem != 0 )
    {
        MT_ValuedListViewItem<ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<ObjectKnowledge*>*)pItem;
        if( pCastItem->GetValue() == &knowledge )
        {
            delete pCastItem;
            return;
        }

        pItem = pItem->nextSibling();
    }
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::OnContextMenuRequested
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::OnContextMenuRequested( QListViewItem* pItem, const QPoint& pos )
{
    if( pItem == 0 ) 
        return;

    pPopupMenu_->clear();

    MT_ValuedListViewItem<ObjectKnowledge*>* pCastItem = (MT_ValuedListViewItem<ObjectKnowledge*>*)pItem;
    SelectedElement selectedElement( *(pCastItem->GetValue()) );
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement ) );

    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::ToggleDisplayOwnTeam
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
void ObjectKnowledgePanel::ToggleDisplayOwnTeam()
{
    this->UpdateList();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgePanel::GetSelectedTeam
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
Team* ObjectKnowledgePanel::GetSelectedTeam( const SelectedElement& /*item*/ )
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
