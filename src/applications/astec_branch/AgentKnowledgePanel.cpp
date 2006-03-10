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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledgePanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: AgentKnowledgePanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledgePanel.h"
#include "moc_AgentKnowledgePanel.cpp"

#include "Agent.h"
#include "AgentKnowledges.h"
#include "AgentKnowledge.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "Controller.h"
#include "ActionController.h"
#include "KnowledgeGroup.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Tools.h"
#include "Units.h"
#include "PerceptionMap.h"
#include "ListDisplayer.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel constructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::AgentKnowledgePanel( InfoPanels* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC     ( pParent, tr( "C. agent" ) )
//    , pPopupMenu_        ( new QPopupMenu( this ) )
    , selected_( 0 )
    , subSelected_( 0 )
    , display_( 0 )
{
    pKnowledgeListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this );
    pKnowledgeListView_->AddColumn( "Agents connus" );
    
//    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
//    pOwnTeamCheckBox_->setChecked( true );
//
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Détails" )
                .AddLabel( "Id:" )
                .AddLabel( "Agent associé:" )
                .AddLabel( "Position:" )
                .AddLabel( "Direction:" )
                .AddLabel( "Vitesse:" )
                .AddLabel( "Etat ops.:" )
                .AddLabel( "Niveau de perception:" )
                .AddLabel( "Niveau max de perception:" )
                .AddLabel( "Camp:" )
                .AddLabel( "Niveau:" )
                .AddLabel( "Arme:" )
                .AddLabel( "Spécialisation:" )
                .AddLabel( "Qualification:" )
                .AddLabel( "Catégorie:" )
                .AddLabel( "Mobilité:" )
                .AddLabel( "Capacité mission:" )
                .AddLabel( "Rendu:" )
                .AddLabel( "Fait prisonnier:" )
                .AddLabel( "Réfugiés pris en compte:" )
                .AddLabel( "PC:" )
                .AddLabel( "Pertinence:"  );

    pPerceptionListView_ = new ListDisplayer< AgentKnowledgePanel >( this, *this );
    pPerceptionListView_->AddColumn( "Agent" ).
                          AddColumn( "Niveau perception" );

//    connect( &App::GetApp(), SIGNAL( AgentKnowledgeCreated( Gtia&, AgentKnowledge& ) ),
//             this,                 SLOT(    OnKnowledgeCreated( Gtia&, AgentKnowledge& ) ) );
//    connect( &App::GetApp(), SIGNAL( AgentKnowledgeUpdated( Gtia&, AgentKnowledge& ) ),
//             this,                 SLOT(    OnKnowledgeUpdated( Gtia&, AgentKnowledge& ) ) );
//    connect( &App::GetApp(), SIGNAL( AgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ),
//             this,                 SLOT(    OnKnowledgeDeleted( Gtia&, AgentKnowledge& ) ) );
//
//    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
//    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),         &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
//    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),             &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
//
//    connect( pOwnTeamCheckBox_,   SIGNAL( clicked() ),                          this, SLOT( ToggleDisplayOwnTeam() ) ); 
    connect( pKnowledgeListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
//    connect( pKnowledgeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint& ) ) );
//    connect( pKnowledgeListView_, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    controller.Register( *this );
    actionController.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel destructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::~AgentKnowledgePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::showEvent( QShowEvent* )
{
    if( selected_ ) 
        NotifyUpdated( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledges& knowledges )
{
    if( ! IsVisible() || selected_ != &knowledges )
        return;

    pKnowledgeListView_->DeleteTail( 
        pKnowledgeListView_->DisplayList( knowledges.CreateIterator() )
        );
    ValuedListItem* item = FindItem( subSelected_, pKnowledgeListView_->firstChild() );
    if( item )
        pKnowledgeListView_->setSelected( item, true );
    else
    {
        subSelected_ = 0;
        pKnowledgeListView_->setSelected( 0, false );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const AgentKnowledge& k, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Agents connus", k.GetRealAgent() );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const KnowledgeGroup* element )
{
    const AgentKnowledges* k = element ? element->Retrieve< AgentKnowledges >() : 0;
    if( ! k || k != selected_ )
    {
        selected_ = k;
        if( selected_ ) {
            subSelected_ = 0;
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( ! item || ! item->IsA< const AgentKnowledge* >() )
        subSelected_ = 0;
    else
        subSelected_ = item->GetValue< const AgentKnowledge* >();

    if( subSelected_ )
    {
        NotifyUpdated( *subSelected_ );
        NotifyUpdated( subSelected_->Get< PerceptionMap >() );
    }
    else
        display_->Hide();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledge& k )
{
    if( ! IsVisible() || subSelected_ != & k )
        return;

    k.Display( display_->Group( "Détails" ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const PerceptionMap& perceptions )
{
    if( ! IsVisible() || ! subSelected_ || subSelected_->Retrieve< PerceptionMap >() != & perceptions )
        return;

    pPerceptionListView_->DeleteTail( 
        pPerceptionListView_->DisplayList( perceptions.perceptions_.begin(), perceptions.perceptions_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const Perception& perception, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Agent", perception.detected_ );
    displayer.Display( "Niveau perception", perception.detected_ );
}
