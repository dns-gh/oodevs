// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_EventToolbar.cpp $
// $Author: Ape $
// $Modtime: 4/10/04 16:54 $
// $Revision: 1 $
// $Workfile: MOS_EventToolbar.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_EventToolbar.h"
#include "moc_MOS_EventToolbar.cpp"

#include "MOS_App.h"
#include "MOS_Options.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_MainWindow.h"
//#include "MOS_Report_ABC.h"
#include "MOS_Team.h"
#include "MOS_EventToolButton.h"
#include "MOS_ActionContext.h"
#include "MOS_SelectedElement.h"

DECLARE_ICON( agentmessage );
DECLARE_ICON( conflict );


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
MOS_EventToolbar::MOS_EventToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "message toolbar" )
{
    this->setLabel( tr( "Messagerie" ) );
    
    MOS_EventToolButton* pB = new MOS_EventToolButton( MAKE_ICON( gas ), "Pannes d'essence", this );
    pB->SetSignalsSlots( this, SIGNAL( AgentOutOfGas( int ) ), SIGNAL( AgentRefueled( int ) ), SLOT( FocusOnAgent( int, bool ) ) );

    MOS_EventToolButton* pB2 = new MOS_EventToolButton( MAKE_ICON( conflict ), "Conflits", this );
    pB2->SetSignalsSlots( this, SIGNAL( ConflictStarted( int ) ), SIGNAL( ConflictEnded( int ) ), SLOT( FocusOnAgent( int, bool ) ) );

    this->addSeparator();

    MOS_EventToolButton* pB3 = new MOS_EventToolButton( MAKE_ICON( msg ), "Général", this );
    pB3->SetSignalsSlots( this, SIGNAL( ReportCreated( int ) ), SIGNAL( ReadingReports( int ) ), SLOT( FocusOnAgent( int, bool ) ) );

    connect( &MOS_App::GetApp(),               SIGNAL( AgentConflictStarted( MOS_Agent& ) ),                    this, SLOT( OnConflictStarted( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(),               SIGNAL( AgentConflictEnded  ( MOS_Agent& ) ),                    this, SLOT( OnConflictEnded  ( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(),               SIGNAL( AgentOutOfGas       ( MOS_Agent& ) ),                    this, SLOT( OnAgentOutOfGas  ( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(),               SIGNAL( AgentRefueled       ( MOS_Agent& ) ),                    this, SLOT( OnAgentRefueled  ( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(),               SIGNAL( ReportCreated       ( MOS_Agent&, MOS_Report_ABC& ) ),   this, SLOT( OnReportCreated  ( MOS_Agent& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ReadingReports      ( MOS_Agent& ) ),                    this, SLOT( OnReadingReports ( MOS_Agent& ) ) );

    connect( &MOS_App::GetApp(),               SIGNAL( ConnexionStatusChanged( bool ) ),                        this, SLOT( ClearSubscriptions() ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                                         this, SLOT( OnTeamChanged() ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), this, SLOT( FillRemotePopup( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),     &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& )) );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar destructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
MOS_EventToolbar::~MOS_EventToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::FillRemotePopup
/** @param  popupMenu 
    @param  context 
    */
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolbar::FillRemotePopup( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.selectedElement_.pAgent_ == 0 )
        return;

    pSelectedAgent_ = context.selectedElement_.pAgent_;
    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
    {
        if( (*it)->GetFilterId() == (int)pSelectedAgent_->GetAgentID() )
        {
            popupMenu.insertItem( tr("Se désabonner"), this, SLOT( UnsubscribeFromAgent() ) );
            return;
        }
    }

    popupMenu.insertItem( tr("S'abonner"), this, SLOT( SubscribeToAgent() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::SubscribeToAgent
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolbar::SubscribeToAgent()
{
    MOS_EventToolButton* pButton = new MOS_EventToolButton( QIconSet(), pSelectedAgent_->GetName().c_str(), this, pSelectedAgent_->GetAgentID() );
    pButton->SetSignalsSlots( this, SIGNAL( ReportCreated( int ) ), SIGNAL( ReadingReports( int ) ), SLOT( FocusOnAgent( int, bool ) ) );

    agentButtons_.push_back( pButton );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::UnsubscribeFromAgent
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolbar::UnsubscribeFromAgent()
{
    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
    {
        if( (*it)->GetFilterId() == (int)pSelectedAgent_->GetAgentID() )
        {
            delete *it;
            agentButtons_.erase( it );
            return;
        }
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnTeamChanged
// Created: APE 2004-09-29
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnTeamChanged()
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
    {
        MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( (*it)->GetFilterId() );
        assert( pAgent != 0 );

        if( (int)( pAgent->GetTeam().GetIdx() ) == options.nPlayedTeam_ )
            (*it)->show();
        else
            (*it)->hide();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::ClearSubscriptions
// Created: APE 2004-10-01
// -----------------------------------------------------------------------------
void MOS_EventToolbar::ClearSubscriptions()
{
    for( IT_ButtonVector it = agentButtons_.begin(); it != agentButtons_.end(); ++it )
        delete *it;
    agentButtons_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnAgentOutOfGas
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnAgentOutOfGas( MOS_Agent& agent )
{
    emit AgentOutOfGas( agent.GetAgentID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnAgentRefueled
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnAgentRefueled( MOS_Agent& agent )
{
    emit AgentRefueled( agent.GetAgentID() );
}

// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnConflictStarted
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnConflictStarted( MOS_Agent& agent )
{
    emit ConflictStarted( agent.GetAgentID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnConflictEnded
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnConflictEnded( MOS_Agent& agent )
{
    emit ConflictEnded( agent.GetAgentID() );
}

// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnReportCreated
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnReportCreated( MOS_Agent& agent )
{
    // Only receive events from agents on the team we are currently playing.
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    if( options.nPlayedTeam_ != MOS_Options::eController && options.nPlayedTeam_ != (int)(agent.GetTeam().GetIdx()) )
        return;
    emit ReportCreated( agent.GetAgentID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::OnReadingReports
/** @param  agent 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::OnReadingReports( MOS_Agent& agent )
{
    emit ReadingReports( agent.GetAgentID() );
}


// -----------------------------------------------------------------------------
// Name: MOS_EventToolbar::FocusOnAgent
/** @param  nId 
    @param  bCenter 
*/
// Created: APE 2004-10-04
// -----------------------------------------------------------------------------
void MOS_EventToolbar::FocusOnAgent( int nId, bool bCenter )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nId );
    assert( pAgent != 0 );
    emit ElementSelected( MOS_SelectedElement( *pAgent ) );
    if( bCenter )
        emit CenterOnPoint( pAgent->GetPos() );
}
