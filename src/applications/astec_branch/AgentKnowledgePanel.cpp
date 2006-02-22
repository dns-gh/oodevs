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

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel constructor
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
AgentKnowledgePanel::AgentKnowledgePanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC     ( pParent, tr( "C. agent" ) )
//    , pPopupMenu_        ( new QPopupMenu( this ) )
    , selected_( 0 )
    , newSelection_( 0 )
    , subSelected_( 0 )
    , display_( 0 )
{
    pKnowledgeListView_ = new ListView< AgentKnowledgePanel >( this, *this );
    pKnowledgeListView_->addColumn( tr( "Agents connus" ) );
    pKnowledgeListView_->setResizeMode( QListView::LastColumn );

//    pOwnTeamCheckBox_ = new QCheckBox( tr( "Afficher propre camp" ), this );
//    pOwnTeamCheckBox_->setChecked( true );
//
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Détails" )
                .AddItem( "Id:" )
                .AddItem( "Agent associé:" )
                .AddItem( "Position:" )
                .AddItem( "Direction:" )
                .AddItem( "Vitesse:" )
                .AddItem( "Etat ops.:" )
                .AddItem( "Niveau de perception:" )
                .AddItem( "Niveau max de perception:" )
                .AddItem( "Camp:" )
                .AddItem( "Niveau:" )
                .AddItem( "Arme:" )
                .AddItem( "Spécialisation:" )
                .AddItem( "Qualification:" )
                .AddItem( "Catégorie:" )
                .AddItem( "Mobilité:" )
                .AddItem( "Capacité mission:" )
                .AddItem( "Rendu:" )
                .AddItem( "Fait prisonnier:" )
                .AddItem( "Réfugiés pris en compte:" )
                .AddItem( "PC:" )
                .AddItem( "Pertinence:"  );

    pPerceptionListView_ = new ListView< AgentKnowledgePanel >( this, *this );
    pPerceptionListView_->addColumn( tr( "Agent" ) );
    pPerceptionListView_->addColumn( tr( "Niveau perception" ) );
    pPerceptionListView_->setResizeMode( QListView::LastColumn );
    pPerceptionListView_->setAllColumnsShowFocus( true );

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
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledges& knowledges )
{
    if( ! isVisible() || selected_ != &knowledges )
        return;

    pKnowledgeListView_->Display( knowledges.CreateIterator(), pKnowledgeListView_, (ValuedListItem*)( pKnowledgeListView_->firstChild() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const AgentKnowledge& k, ValuedListItem* item )
{
    item->SetValue( &k );
    item->setText( 0, k.GetRealAgent().GetName().c_str() ); // $$$$ AGE 2006-02-21: plus que l'agent !
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::BeforeSelection
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::BeforeSelection()
{
    newSelection_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::AfterSelection
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::AfterSelection()
{
    if( selected_ != newSelection_ )
    {
        selected_ = newSelection_;
        if( selected_ )
        {
            Show();
            NotifyUpdated( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const KnowledgeGroup& element )
{
    Select( element.Retrieve< AgentKnowledges >() );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const Agent& element )
{
    const KnowledgeGroup* kg = element.GetKnowledgeGroup();
    if( kg )
        Select( *kg );
    else
        Select( 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Select
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Select( const AgentKnowledges* k )
{
    newSelection_ = k;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::OnSelectionChanged
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::OnSelectionChanged( QListViewItem* i )
{
    ValuedListItem* item = (ValuedListItem*)( i );
    if( ! item || ! item->IsA< const AgentKnowledge* >() )
        return;
    subSelected_ = item->GetValue< const AgentKnowledge* >();
    if( subSelected_ )
    {
        NotifyUpdated( *subSelected_ );
        NotifyUpdated( subSelected_->Get< PerceptionMap >() );
    }
    else
        display_->Clear();
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const AgentKnowledge& k )
{
    if( ! isVisible() || subSelected_ != & k )
        return;

    display_->Group( "Détails" )
                .Display( "Id:", k.GetId() )
                .Display( "Agent associé:", k.GetRealAgent() )
                .Display( "Position:", k.strPosition_ )
                .Display( "Direction:", k.nDirection_ * Units::degrees )
                .Display( "Vitesse:", k.nSpeed_ * Units::metersPerSecond )
                .Display( "Etat ops.:", k.nEtatOps_ )
                .Display( "Niveau de perception:", k.nCurrentPerceptionLevel_  )
                .Display( "Niveau max de perception:", k.nMaxPerceptionLevel_ )
//                .Display( "Camp:", IfSet( k.nTeam_, App::GetApp().GetAgentManager().FindTeam( k.nTeam_)->GetName() ) )
                .Display( "Niveau:", k.nLevel_ )
                .Display( "Arme:", k.nWeapon_ )
                .Display( "Spécialisation:", k.nSpecialization_ )
                .Display( "Qualification:", k.nQualifier_  )
                .Display( "Catégorie:", k.nCategory_ )
                .Display( "Mobilité:", k.nMobility_  )
                .Display( "Capacité mission:", k.nCapacity_ )
                .Display( "Rendu:", k.bSurrendered_ )
                .Display( "Fait prisonnier:", k.bPrisonner_ )
                .Display( "Réfugiés pris en compte:", k.bRefugies_ )
                .Display( "PC:", k.bIsPC_ )
                .Display( "Pertinence:", k.nRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::NotifyUpdated
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::NotifyUpdated( const PerceptionMap& perceptions )
{
    if( ! isVisible() || ! subSelected_ || subSelected_->Retrieve< PerceptionMap >() != & perceptions )
        return;

    pPerceptionListView_->Display( perceptions.perceptions_.begin(), perceptions.perceptions_.end(),
            pPerceptionListView_, (ValuedListItem*)( pPerceptionListView_->firstChild() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePanel::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void AgentKnowledgePanel::Display( const Perception& perception, ValuedListItem* item )
{
    item->SetValue( perception );
    item->setText( 0, perception.detected_->GetName().c_str() );
    item->setText( 1, Tools::ToString( perception.level_ ) );
}
