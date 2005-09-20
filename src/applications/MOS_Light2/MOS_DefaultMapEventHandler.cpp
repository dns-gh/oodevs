// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DefaultMapEventHandler.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:49 $
// $Revision: 14 $
// $Workfile: MOS_DefaultMapEventHandler.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_DefaultMapEventHandler.h"
#include "moc_MOS_DefaultMapEventHandler.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "MOS_AgentManager.h"
#include "MOS_Object_ABC.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_ObjectManager.h"
#include "MOS_ActionContext.h"
#include "MOS_TacticalLine_ABC.h"
#include "MOS_LineManager.h"
#include "MOS_Tools.h"
#include "MOS_GLTool.h"
#include "MOS_Options.h"
#include "MOS_Team.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_MapMouseEvent.h"
#include "MOS_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler constructor
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
MOS_DefaultMapEventHandler::MOS_DefaultMapEventHandler( QObject* pParent )
    : QObject                ( pParent )
    , MOS_MapEventFilter_ABC ()
    , selectedElement_       ()
    , pPopupMenu_            ( 0 )
{
    pLineEditor_ = new MOS_ShapeEditorMapEventFilter( this );
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( LineCreated() ) );

    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( ClearSelection() ) );

    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( ClearSelection() ) );

    connect( &MOS_App::GetApp(), SIGNAL( ObjectDeleted( MOS_Object_ABC& ) ), this, SLOT( OnObjectDeleted( MOS_Object_ABC& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TacticalLineDeleted( MOS_TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( MOS_TacticalLine_ABC& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler destructor
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
MOS_DefaultMapEventHandler::~MOS_DefaultMapEventHandler()
{
    // Deletion order is important, as both the pLineEditor and this can pop themselves off
    // the handler list. The pLineEditor must be popped first.
    delete pLineEditor_;

    delete pPopupMenu_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::Draw
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::Draw() const
{
    MOS_GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::eventFilter
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( *(QKeyEvent*)pEvent );

        // All QMouseEvent are actually MOS_MapMouseEvent. Cast and use them.
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
            return OnMousePress( *(MOS_MapMouseEvent*)pEvent );
            
        case QEvent::MouseMove:
            return OnMouseMove( *(MOS_MapMouseEvent*)pEvent );

        case QEvent::MouseButtonRelease:
            return OnMouseRelease( *(MOS_MapMouseEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnMousePress
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::OnMousePress( const MOS_MapMouseEvent& mouseEvent )
{
    vOldMousePos_ = mouseEvent.GetMapPos();
    if( mouseEvent.button() == LeftButton && mouseEvent.state() == NoButton )
    {
        SelectElementAtPos( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel() );

        // Disconnect and connect to avoid having the signal come back to us.
        disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
        emit ElementSelected( selectedElement_ );
        connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
        return false;
    }

    if( mouseEvent.button() == RightButton && mouseEvent.state() == NoButton )
    {
        PopupMenu( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel() );
        return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnMouseRelease
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::OnMouseRelease( const MOS_MapMouseEvent& mouseEvent )
{
    if(      mouseEvent.state() == (LeftButton | ShiftButton) 
        &&   selectedElement_.pLine_ != 0
        && ! selectedElement_.pLine_->IsUpdatingToSim() )
    {
        selectedElement_.pLine_->UpdateToSim();
        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnMouseMove
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::OnMouseMove( const MOS_MapMouseEvent& mouseEvent )
{
    // If moving a tactical line's point around...
    if( mouseEvent.state() == (LeftButton | ShiftButton) )
    {
        if(  selectedElement_.nLinePoint_ != -1 
        && (! selectedElement_.pLine_->IsUpdatingToSim() || MOS_App::GetApp().IsODBEdition() ) )
        {
            selectedElement_.pLine_->ModifyPoint( selectedElement_.nLinePoint_, mouseEvent.GetMapPos() );
            vOldMousePos_ = mouseEvent.GetMapPos();
            return true;
        }

    // If moving a tactical line around...
        if(      selectedElement_.pLine_ != 0 
        &&   selectedElement_.nLinePoint_ == -1 
        && (! selectedElement_.pLine_->IsUpdatingToSim() || MOS_App::GetApp().IsODBEdition() ) )
        {
           selectedElement_.pLine_->Translate( mouseEvent.GetMapPos() - vOldMousePos_ );
           vOldMousePos_ = mouseEvent.GetMapPos();
           return true;
        }
    }
    vOldMousePos_ = mouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnKeyPress
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::OnKeyPress( const QKeyEvent& keyEvent )
{
    switch( keyEvent.key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
        {
            // If we're trying to delete a line, and the line is not already
            // trying to update itself to the sim, request its deletion.
            if( selectedElement_.pLine_ != 0  && ! selectedElement_.pLine_->IsUpdatingToSim() )
            {
                selectedElement_.pLine_->SetState( MOS_TacticalLine_ABC::eStateDeleted );
                selectedElement_.pLine_->UpdateToSim();
                return true;
            }
            
            // If we're trying to delete a line when editing an odb, delete it
            // immediatly.
            if( selectedElement_.pLine_ != 0  && MOS_App::GetApp().IsODBEdition() )
            {
                MOS_App::GetApp().GetLineManager().DeleteLine( selectedElement_.pLine_->GetID() );
                selectedElement_.pLine_ = 0;
                selectedElement_.nLinePoint_ = -1;
                return true;
            }
        }
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::SwitchToLineCreation
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::SwitchToLineCreation( int nType )
{
    nLineType_ = nType;
    pLineEditor_->PrepareNewLine( MOS_ShapeEditorMapEventFilter::eTacticalLine );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::LineCreated
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::LineCreated()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.size() < 2 )
        return;

    if( nLineType_ == 999 )
    {
        // It's a limit.
        MOS_Limit* pLimit = new MOS_Limit( pointList );
        MOS_App::GetApp().GetLineManager().RegisterLine( *pLimit );
        if( ! MOS_App::GetApp().IsODBEdition() )
            pLimit->UpdateToSim();
    }
    else
    {
        // It's a lima, and nLineType_ is the lima's type.
        MOS_Lima* pLima = new MOS_Lima( pointList, (E_FuncLimaType)nLineType_ );
        MOS_App::GetApp().GetLineManager().RegisterLine( *pLima );
        if( ! MOS_App::GetApp().IsODBEdition() )
            pLima->UpdateToSim();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::ClearSelection
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::ClearSelection()
{
    selectedElement_ = MOS_SelectedElement();
    emit ElementSelected( selectedElement_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnObjectDeleted
/** @param  object 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnObjectDeleted( MOS_Object_ABC& object )
{
    if( selectedElement_.pObject_ == &object )
    {
        selectedElement_ = MOS_SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnTacticalLineDeleted
/** @param  line 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnTacticalLineDeleted( MOS_TacticalLine_ABC& line )
{
    if( selectedElement_.pLine_ == &line )
    {
        selectedElement_ = MOS_SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnAgentKnowledgeDeleted
/** @param  knowledge 
*/
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnAgentKnowledgeDeleted( MOS_Gtia& /*gtia*/, MOS_AgentKnowledge& knowledge )
{
    if( selectedElement_.pAgentKnowledge_ == &knowledge )
    {
        selectedElement_ = MOS_SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnObjectKnowledgeDeleted
/** @param  knowledge 
*/
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnObjectKnowledgeDeleted( MOS_Team& /*team*/, MOS_ObjectKnowledge& knowledge )
{
    if( selectedElement_.pObjectKnowledge_ == &knowledge )
    {
        selectedElement_ = MOS_SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::SelectElementAtPos
/** @param  vGLPos 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::SelectElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_Agent* pAgent = GetAgentAtPos( vGLPos );
    if( pAgent != 0 )
    {
        selectedElement_ = MOS_SelectedElement( *pAgent );
        return;
    }

    if( selectedElement_.pLine_ != 0 )
    {
        int nPoint = GetPointAtPos( *selectedElement_.pLine_, vGLPos, rDistancePerPixel );
        if( nPoint != -1 )
        {
            selectedElement_ = MOS_SelectedElement( *selectedElement_.pLine_, nPoint );
            return;
        }
    }

    MOS_TacticalLine_ABC* pLine = GetLineAtPos( vGLPos, rDistancePerPixel );
    if( pLine != 0 )
    {
        selectedElement_ = MOS_SelectedElement( *pLine );
        return;
    }

    // Only enable knowledge selection in non all-team mode.
    if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != MOS_Options::eController )
    {
        MOS_AgentKnowledge* pAgentKnowledge = GetAgentKnowledgeAtPos( vGLPos );
        if( pAgentKnowledge != 0 )
        {
            selectedElement_ = MOS_SelectedElement( *pAgentKnowledge );
            return;
        }
    }
   
    // Only enable direct object selection in all-team mode.
    if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ == MOS_Options::eController )
    {
        MOS_Object_ABC* pObject = GetObjectAtPos( vGLPos, rDistancePerPixel );
        if( pObject != 0 )
        {
            selectedElement_ = MOS_SelectedElement( *pObject );
            return;
        }
    }

    // Only enable knowledge selection in non all-team mode.
    if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != MOS_Options::eController )
    {
        MOS_ObjectKnowledge* pObjectKnowledge = GetObjectKnowledgeAtPos( vGLPos, rDistancePerPixel );
        if( pObjectKnowledge != 0 )
        {
            selectedElement_ = MOS_SelectedElement( *pObjectKnowledge );
            return;
        }
    }

    selectedElement_ = MOS_SelectedElement();
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetAgentAtPos
/** @param  vPos 
    @return 
*/
// Created: APE 2004-03-09
// -----------------------------------------------------------------------------
MOS_Agent* MOS_DefaultMapEventHandler::GetAgentAtPos( const MT_Vector2D& vGLPos )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    MOS_AgentManager::CT_AgentMap& agentMap = MOS_App::GetApp().GetAgentManager().GetAgentList();
    MOS_AgentManager::RCIT_AgentMap rit;

    if( selectedElement_.pAgent_ == 0 )
    {
        rit = agentMap.rbegin();
    }
    else
    {
        MOS_AgentManager::CIT_AgentMap it = agentMap.find( selectedElement_.pAgent_->GetAgentID() );
        assert( it != agentMap.end() );
        rit = MOS_AgentManager::RCIT_AgentMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < agentMap.size(); ++n )
    {
        if( rit == agentMap.rend() )
            rit = agentMap.rbegin();

        MOS_Agent* pAgent = (*rit).second;
        if( nPlayedTeam == MOS_Options::eController || nPlayedTeam == (int)(pAgent->GetTeam().GetIdx()) )
        {
            if( IsAgentAtPos( *pAgent, vGLPos ) )
                return pAgent;
        }

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetAgentKnowledgeAtPos
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_AgentKnowledge* MOS_DefaultMapEventHandler::GetAgentKnowledgeAtPos( const MT_Vector2D& vGLPos )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    assert( nPlayedTeam != MOS_Options::eController );  // You can't select agent knowledges on the map when in 'all teams' mode (ie. controller view).

    MOS_Team* pPlayedTeam = MOS_App::GetApp().GetAgentManager().FindTeamFromIdx( nPlayedTeam );
    assert( pPlayedTeam != 0 );

    const MOS_Team::T_AgentKnowledgeMap& agentMap = pPlayedTeam->GetAgentKnowledges();
    MOS_Team::RCIT_AgentKnowledgeMap rit;

    if( selectedElement_.pAgentKnowledge_ == 0 )
    {
        rit = agentMap.rbegin();
    }
    else
    {
        MOS_Team::CIT_AgentKnowledgeMap it = agentMap.find( selectedElement_.pAgentKnowledge_->GetID() );
        assert( it != agentMap.end() );
        rit = MOS_Team::RCIT_AgentKnowledgeMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < agentMap.size(); ++n )
    {
        if( rit == agentMap.rend() )
            rit = agentMap.rbegin();

        MOS_AgentKnowledge* pAgentKnowledge = (*rit).second;
        // Only allow the selection of agent knowledges pointing to agents not in our team.
        if( pAgentKnowledge->GetRealAgent().GetTeam().GetID() != pPlayedTeam->GetID() )
        {
            if( IsAgentKnowledgeAtPos( *pAgentKnowledge, vGLPos ) )
                return pAgentKnowledge;
        }

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetLineAtPos
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_TacticalLine_ABC* MOS_DefaultMapEventHandler::GetLineAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_LineManager::CT_TacticalLineSet& lineSet = MOS_App::GetApp().GetLineManager().GetLineList();
    MOS_LineManager::RCIT_TacticalLineSet rit;

    if( selectedElement_.pLine_ == 0 )
    {
        rit = lineSet.rbegin();
    }
    else
    {
        MOS_LineManager::CIT_TacticalLineSet it = lineSet.find( selectedElement_.pLine_ );
        assert( it != lineSet.end() );
        rit = MOS_LineManager::RCIT_TacticalLineSet( ++it );
        ++rit;
    }

    for( uint n = 0; n < lineSet.size(); ++n )
    {
        if( rit == lineSet.rend() )
            rit = lineSet.rbegin();

        MOS_TacticalLine_ABC* pLine = (*rit);
        if( IsLineAtPos( *pLine, vGLPos, rDistancePerPixel ) )
            return pLine;

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsLineAtPos
/** @param  agent 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsLineAtPos( const MOS_TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    CT_PointVector& pointList = line.GetPointList();
    assert( pointList.size() >= 2 );

    return MOS_Tools::PointNearLine( vGLPos, pointList, 9.0 * rDistancePerPixel );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetPointAtPos
/** @param  line 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
int MOS_DefaultMapEventHandler::GetPointAtPos( MOS_TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    CT_PointVector& pointList = line.GetPointList();
    for( uint n = 0; n < pointList.size(); ++n )
    {
        if( vGLPos.SquareDistance( pointList[n] ) < (5.0*5.0)*(rDistancePerPixel*rDistancePerPixel) )
            return n;
    }
    return -1;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetObjectAt
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_Object_ABC* MOS_DefaultMapEventHandler::GetObjectAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    const MOS_ObjectManager::T_ObjectMap& objectMap = MOS_App::GetApp().GetObjectManager().GetObjects();
    MOS_ObjectManager::RCIT_ObjectMap rit;

    if( selectedElement_.pObject_ == 0 )
    {
        rit = objectMap.rbegin();
    }
    else
    {
        MOS_ObjectManager::CIT_ObjectMap it = objectMap.find( selectedElement_.pObject_->GetID() );
        assert( it != objectMap.end() );
        rit = MOS_ObjectManager::RCIT_ObjectMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < objectMap.size(); ++n )
    {
        if( rit == objectMap.rend() )
            rit = objectMap.rbegin();

        MOS_Object_ABC* pObject = (*rit).second;
        if( IsObjectAtPos( *pObject, vGLPos, rDistancePerPixel ) )
            return pObject;

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsObjectAtPos
/** @param  object 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsObjectAtPos( const MOS_Object_ABC& object, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    return MOS_Tools::PointNearLine( vGLPos, object.GetPointList(), 9.0 * rDistancePerPixel, object.GetLocationType() == EnumTypeLocalisation::polygon );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetObjectKnowledgeAtPos
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_ObjectKnowledge* MOS_DefaultMapEventHandler::GetObjectKnowledgeAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    assert( options.nPlayedTeam_ != MOS_Options::eController );
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeamFromIdx( options.nPlayedTeam_ );
    assert( pTeam != 0 );
    const MOS_Team::T_ObjectKnowledgeMap& objectMap = pTeam->GetObjectKnowledges();

    MOS_Team::RCIT_ObjectKnowledgeMap rit;

    if( selectedElement_.pObjectKnowledge_ == 0 )
    {
        rit = objectMap.rbegin();
    }
    else
    {
        MOS_Team::CIT_ObjectKnowledgeMap it = objectMap.find( selectedElement_.pObjectKnowledge_->GetID() );
        assert( it != objectMap.end() );
        rit = MOS_Team::RCIT_ObjectKnowledgeMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < objectMap.size(); ++n )
    {
        if( rit == objectMap.rend() )
            rit = objectMap.rbegin();

        MOS_ObjectKnowledge* pObject = (*rit).second;
        if( IsObjectKnowledgeAtPos( *pObject, vGLPos, rDistancePerPixel ) )
            return pObject;

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsObjectKnowledgeAtPos
/** @param  object 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsObjectKnowledgeAtPos( const MOS_ObjectKnowledge& object, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_Object_ABC* pObject = object.GetRealObject();
    if( pObject != 0 )
        return IsObjectAtPos( *pObject, vGLPos, rDistancePerPixel );
    else
        return MOS_Tools::PointNearLine( vGLPos, object.GetPointList(), 9.0 * rDistancePerPixel );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::PopupMenu
/** @param  vPos 
*/
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::PopupMenu( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    // If something is selected, make sure we clicked on it, otherwise select what's under the click.
    // If nothing selected, try selecting what's under the click.
    if(   ( ! selectedElement_.IsAMapElementSelected() )
       || ( selectedElement_.pAgent_          != 0 && ! IsAgentAtPos( *selectedElement_.pAgent_, vGLPos ) )
       || ( selectedElement_.pObject_     != 0 && ! IsObjectAtPos( *selectedElement_.pObject_, vGLPos, rDistancePerPixel ) )
       || ( selectedElement_.pLine_           != 0 && ! IsLineAtPos( *selectedElement_.pLine_, vGLPos, rDistancePerPixel ) )
       || ( selectedElement_.pAgentKnowledge_ != 0 && ! IsAgentKnowledgeAtPos( *selectedElement_.pAgentKnowledge_, vGLPos ) ) )
    {
        SelectElementAtPos( vGLPos, rDistancePerPixel );

        // Disconnect and connect to avoid having the signal come back to us.
        disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
        emit ElementSelected( selectedElement_ );
        connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    }

    // Delete the old menu (necessary for correctly deleting any submenus).
    delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu();

    MT_Vector2D vGLPos2 = vGLPos;
    // Give other interface elements a chance to fill the popup menu.
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement_, &vGLPos2 ) );

    // Add all the lima / limit creation commands to the popup menu.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->insertSeparator();

    int nId = pPopupMenu_->insertItem( tr( "Créer limite" ), this, SLOT( SwitchToLineCreation( int ) ) );
    pPopupMenu_->setItemParameter( nId, 999 );

    QPopupMenu* pCreateLimaMenu = new QPopupMenu( pPopupMenu_ );
    for( int n = 0; n < eLimaFuncNbr; ++n )
    {
        int nId = pCreateLimaMenu->insertItem( MOS_Tools::ToString( (E_FuncLimaType)n ), this, SLOT( SwitchToLineCreation( int ) ) ); 
        pCreateLimaMenu->setItemParameter( nId, n );
    }
    pPopupMenu_->insertItem( tr( "Créer lima" ), pCreateLimaMenu );

    // Finally popup the menu.
    pPopupMenu_->popup( QCursor::pos() );
}
