// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EventToolbar.h"
#include "moc_EventToolbar.cpp"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: EventToolbar constructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::EventToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "event toolbar" )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar destructor
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
EventToolbar::~EventToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::NotifyContextMenu
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void EventToolbar::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    CIT_Agents it = std::find( subscribed_.begin(), subscribed_.end(), &agent );
    if( menu.count() > 0 )
        menu.insertSeparator();
    selected_ = &agent;
    if( it == subscribed_.end() )
        menu.insertItem( "S'abonner", this, SLOT( Subscribe() ) );
    else
        menu.insertItem( "Se désabonner", this, SLOT( UnSubscribe() ) );
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::Subscribe
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void EventToolbar::Subscribe()
{
    if( !selected_ )
        return;
    CIT_Agents it = std::find( subscribed_.begin(), subscribed_.end(), selected_ );
    if( it != subscribed_.end() )
        return;
    subscribed_.push_back( selected_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventToolbar::UnSubscribe
// Created: SBO 2006-06-20
// -----------------------------------------------------------------------------
void EventToolbar::UnSubscribe()
{
    if( !selected_ )
        return;
    CIT_Agents it = std::find( subscribed_.begin(), subscribed_.end(), selected_ );
    if( it == subscribed_.end() )
        return;
    std::swap( subscribed_.back(), (const Agent*)*it );
    subscribed_.pop_back();
    selected_ = 0;
}

//
//
//
//
//
//
//
//
//
//
//
//
//#include "App.h"
//#include "Options.h"
//#include "AgentManager.h"
//#include "Agent_ABC.h"
//#include "Agent.h"
//#include "Population.h"
//#include "MainWindow.h"
//#include "Team.h"
//#include "EventToolButton.h"
//#include "ActionContext.h"
//#include "SelectedElement.h"
//#include "Model.h"
//
//DECLARE_ICON( agentmessage );
//DECLARE_ICON( conflict );
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar constructor
//// Created: APE 2004-09-21
//// -----------------------------------------------------------------------------
//EventToolbar::EventToolbar( QMainWindow* pParent )
//    : QToolBar( pParent, "message toolbar" )
//{
//    this->setLabel( tr( "Messagerie" ) );
//    
//    EventToolButton* pB = new EventToolButton( MAKE_ICON( gas ), "Pannes d'essence", this );
//    pB->SetSignalsSlots( this, SIGNAL( AgentOutOfGas( int ) ), SIGNAL( AgentRefueled( int ) ), SLOT( FocusOnAgent( int, bool ) ) );
//
//    EventToolButton* pB2 = new EventToolButton( MAKE_ICON( conflict ), "Conflits", this );
//    pB2->SetSignalsSlots( this, SIGNAL( ConflictStarted( int ) ), SIGNAL( ConflictEnded( int ) ), SLOT( FocusOnAgent( int, bool ) ) );
//
//    this->addSeparator();
//
//    EventToolButton* pB3 = new EventToolButton( MAKE_ICON( msg ), "Général", this );
//    pB3->SetSignalsSlots( this, SIGNAL( ReportCreated( int ) ), SIGNAL( ReadingReports( int ) ), SLOT( FocusOnAgent( int, bool ) ) );
//
//    connect( &App::GetApp(),               SIGNAL( ConflictStarted( Agent_ABC& ) ),                  this, SLOT( OnConflictStarted( Agent_ABC& ) ) );
//    connect( &App::GetApp(),               SIGNAL( ConflictEnded  ( Agent_ABC& ) ),                  this, SLOT( OnConflictEnded  ( Agent_ABC& ) ) );
//    connect( &App::GetApp(),               SIGNAL( AgentOutOfGas  ( Agent& ) ),                      this, SLOT( OnAgentOutOfGas  ( Agent& ) ) );
//    connect( &App::GetApp(),               SIGNAL( AgentRefueled  ( Agent& ) ),                      this, SLOT( OnAgentRefueled  ( Agent& ) ) );
//    connect( &App::GetApp(),               SIGNAL( ReportCreated  ( Agent_ABC&, Report_ABC& ) ), this, SLOT( OnReportCreated  ( Agent_ABC& ) ) );
//    connect( &MainWindow::GetMainWindow(), SIGNAL( ReadingReports ( Agent_ABC& ) ),                  this, SLOT( OnReadingReports ( Agent_ABC& ) ) );
//
//    connect( &App::GetApp(),               SIGNAL( ConnexionStatusChanged( bool ) ),                        this, SLOT( ClearSubscriptions() ) );
//    connect( &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                                         this, SLOT( OnTeamChanged() ) );
//    connect( &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), this, SLOT( FillRemotePopup( QPopupMenu&, const ActionContext& ) ) );
//    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),     &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
//    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ), &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& )) );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar destructor
//// Created: APE 2004-09-21
//// -----------------------------------------------------------------------------
//EventToolbar::~EventToolbar()
//{
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::FillRemotePopup
//// Created: APE 2004-09-29
//// -----------------------------------------------------------------------------
//void EventToolbar::FillRemotePopup( QPopupMenu& popupMenu, const ActionContext& context )
//{
//    if( context.selectedElement_.pAgent_ == 0 )
//        return;
//
//    pSelectedAgent_ = context.selectedElement_.pAgent_;
//    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
//    {
//        if( (*it)->GetFilterId() == (int)pSelectedAgent_->GetID() )
//        {
//            popupMenu.insertItem( tr("Se désabonner"), this, SLOT( UnsubscribeFromAgent() ) );
//            return;
//        }
//    }
//
//    popupMenu.insertItem( tr("S'abonner"), this, SLOT( SubscribeToAgent() ) );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::SubscribeToAgent
//// Created: APE 2004-09-29
//// -----------------------------------------------------------------------------
//void EventToolbar::SubscribeToAgent()
//{
//    EventToolButton* pButton = new EventToolButton( QIconSet(), pSelectedAgent_->GetName().c_str(), this, pSelectedAgent_->GetID() );
//    pButton->SetSignalsSlots( this, SIGNAL( ReportCreated( int ) ), SIGNAL( ReadingReports( int ) ), SLOT( FocusOnAgent( int, bool ) ) );
//    pSelectedAgent_->bListened_ = true;
//    agentButtons_.push_back( pButton );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::UnsubscribeFromAgent
//// Created: APE 2004-09-29
//// -----------------------------------------------------------------------------
//void EventToolbar::UnsubscribeFromAgent()
//{
//    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
//    {
//        if( (*it)->GetFilterId() == (int)pSelectedAgent_->GetID() )
//        {
//            delete *it;
//            agentButtons_.erase( it );
//            pSelectedAgent_->bListened_ = false;
//            return;
//        }
//    }
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnTeamChanged
//// Created: APE 2004-09-29
//// -----------------------------------------------------------------------------
//void EventToolbar::OnTeamChanged()
//{
//    Options& options = MainWindow::GetMainWindow().GetOptions();
//    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
//    {
//        Agent* pAgent = & App::GetApp().GetModel().GetAgent( (*it)->GetFilterId() );
//
//        if( (int)( pAgent->GetTeam().GetIdx() ) == options.nPlayedTeam_ )
//            (*it)->show();
//        else
//            (*it)->hide();
//    }
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::ClearSubscriptions
//// Created: APE 2004-10-01
//// -----------------------------------------------------------------------------
//void EventToolbar::ClearSubscriptions()
//{
//    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
//        delete *it;
//    agentButtons_.clear();
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnAgentOutOfGas
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnAgentOutOfGas( Agent& agent )
//{
//    emit AgentOutOfGas( agent.GetID() );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnAgentRefueled
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnAgentRefueled( Agent& agent )
//{
//    emit AgentRefueled( agent.GetID() );
//}
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnConflictStarted
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnConflictStarted( Agent_ABC& origin )
//{
//    emit ConflictStarted( origin.GetID() );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnConflictEnded
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnConflictEnded( Agent_ABC& origin )
//{
//    emit ConflictEnded( origin.GetID() );
//}
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnReportCreated
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnReportCreated( Agent_ABC& agent )
//{
//    // Only receive events from agents on the team we are currently playing.
//    Options& options = MainWindow::GetMainWindow().GetOptions();
//    if( options.nPlayedTeam_ != Options::eController && options.nPlayedTeam_ != (int)(agent.GetTeam().GetIdx()) )
//        return;
//    emit ReportCreated( agent.GetID() );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::OnReadingReports
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::OnReadingReports( Agent_ABC& agent )
//{
//    emit ReadingReports( agent.GetID() );
//}
//
//
//// -----------------------------------------------------------------------------
//// Name: EventToolbar::FocusOnAgent
//// Created: APE 2004-10-04
//// -----------------------------------------------------------------------------
//void EventToolbar::FocusOnAgent( int nId, bool bCenter )
//{
//    Agent_ABC*      pOrigin = 0;
//    SelectedElement newSelection;
//
//    // $$$$ AGE 2006-02-13: 
//    Agent* pAgent = App::GetApp().GetModel().FindAgent( nId );
//    if( pAgent )
//    {
//        newSelection.pAgent_ = pAgent;
//        pOrigin = pAgent;
//    }
//    else
//    {
//        Population* pPopulation = App::GetApp().GetModel().FindPopulation( nId );
//        if( pPopulation )
//        {
//            newSelection.pPopulation_ = pPopulation;
//            pOrigin = pPopulation;
//        }
//
//    }
//    emit ElementSelected( newSelection );
//    if( bCenter && pOrigin )
//        emit CenterOnPoint( pOrigin->GetPos() );
//}
