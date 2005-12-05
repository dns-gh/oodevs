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
#include "MOS_Population.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationFlow.h"

#include "MT_Tools/MT_Circle.h"


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler constructor
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
MOS_DefaultMapEventHandler::MOS_DefaultMapEventHandler( QObject* pParent )
    : QObject                ( pParent )
    , MOS_MapEventFilter_ABC ()
    , selectedElement_       ()
    , pPopupMenu_            ( 0 )
    , nSelectionState         ( ePhaseSelectionAgent )
{
    pLineEditor_ = new MOS_ShapeEditorMapEventFilter( this );
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( LineCreated() ) );

    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );
    connect( this, SIGNAL( ElementHovered( MOS_SelectedElement& ) ),                 &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementHovered( MOS_SelectedElement& ) ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( ClearSelection() ) );

    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( ClearSelection() ) );

    connect( &MOS_App::GetApp(), SIGNAL( ObjectDeleted( MOS_Object_ABC& ) ), this, SLOT( OnObjectDeleted( MOS_Object_ABC& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationConcentrationDeleted( MOS_PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationDeleted( MOS_PopulationConcentration& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( PopulationFlowDeleted( MOS_PopulationFlow& ) ), this, SLOT( OnPopulationFlowDeleted( MOS_PopulationFlow& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( TacticalLineDeleted( MOS_TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( MOS_TacticalLine_ABC& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( MOS_Gtia&, MOS_AgentKnowledge& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( MOS_Team&, MOS_ObjectKnowledge& ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

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
    bool bAutomata = mouseEvent.state() == ShiftButton || mouseEvent.state() == AltButton;
    if( mouseEvent.button() == LeftButton && ( mouseEvent.state() ==  NoButton || mouseEvent.state() == ShiftButton || mouseEvent.state() == AltButton ) )
    {
        SelectElementAtPos( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel(), bAutomata );
        //if needed center on the automata
        if( selectedElement_.pAgent_ && bAutomata && mouseEvent.state() == AltButton )
        {
            emit CenterOnPoint( selectedElement_.pAgent_->GetPos() );
        }
        //connect and connect to avoid having the signal come back to us.
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
    
    //deal with over events, the test is only here for performance
    if( MOS_MainWindow::GetMainWindow().GetOptions().bDisplayHoveredInfo_ )
        HoverElementAtPos( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel() ) ;
    
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
// Name: MOS_DefaultMapEventHandler::OnPopulationConcentrationDeleted
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnPopulationConcentrationDeleted( MOS_PopulationConcentration& concentration )
{
    if( selectedElement_.pPopulationConcentration_ == &concentration )
    {
        selectedElement_ = MOS_SelectedElement();
        selectedElement_.pPopulation_ = const_cast< MOS_Population* >( &concentration.GetPopulation() );
        emit ElementSelected( selectedElement_ );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnPopulationFlowDeleted
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::OnPopulationFlowDeleted( MOS_PopulationFlow& flow )
{
    if( selectedElement_.pPopulationFlow_ == &flow )
    {
        selectedElement_ = MOS_SelectedElement();
        selectedElement_.pPopulation_ = const_cast< MOS_Population* >( &flow.GetPopulation() );
        emit ElementSelected( selectedElement_ );
    }    
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::OnTacticalLineDeleted
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
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::SelectElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel , bool bAutomata )
{
    //state machine for selection of things, the selection will loop over all overlapped things
    int nBeginningState = nSelectionState;
    bool bLoopDone = false;
    MOS_Agent* pAgent = 0;
    MOS_Object_ABC* pObject = 0;
    MOS_ObjectKnowledge* pObjectKnowledge = 0;
    MOS_AgentKnowledge* pAgentKnowledge = 0;
    MOS_PopulationConcentration* pPopulationConcentration = 0;
    MOS_PopulationFlow* pPopulationFlow = 0;
    //search while a whole loop over the overlapped things has been done
    while ( nSelectionState != nBeginningState || ! bLoopDone )
    {
        switch( nSelectionState )
        {
        case ePhaseSelectionAgent:
            if ( !GetAgentAtPos( pAgent, vGLPos ) && pAgent )
            {
                if( bAutomata && ( ! pAgent->IsAutomate() ) )
                    selectedElement_ = MOS_SelectedElement( *pAgent->GetParent() );
                else
                    selectedElement_ = MOS_SelectedElement( *pAgent );
                return;
            }
            nSelectionState = ePhaseSelectionObject;
            break;
        case ePhaseSelectionObject:
            if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != MOS_Options::eController )
            {
                nSelectionState = ePhaseSelectionAgentKnowledge;
                break;
            }
            if ( ! GetObjectAtPos( pObject, vGLPos, rDistancePerPixel ) && pObject )
            {
                selectedElement_ = MOS_SelectedElement( *pObject );
                return;
            }
            nSelectionState = ePhaseSelectionAgentKnowledge;;
            break;
        case ePhaseSelectionAgentKnowledge:
            if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ == MOS_Options::eController )
            {
                nSelectionState = ePhaseSelectionObjectKnowledge;
                break;
            }
            if ( !GetAgentKnowledgeAtPos( pAgentKnowledge, vGLPos ) && pAgentKnowledge )
            {
                selectedElement_ = MOS_SelectedElement( *pAgentKnowledge );
                return;
            }
            nSelectionState = ePhaseSelectionObjectKnowledge;
            break;
        case ePhaseSelectionObjectKnowledge:
            if( MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ == MOS_Options::eController )
            {
                nSelectionState = ePhaseSelectionPopulationConcentration;
                break;
            }
            if ( !GetObjectKnowledgeAtPos( pObjectKnowledge, vGLPos, rDistancePerPixel ) && pObjectKnowledge )
            {
                selectedElement_ = MOS_SelectedElement( *pObjectKnowledge );
                return;
            }
            nSelectionState = ePhaseSelectionPopulationConcentration;
            break;
        case ePhaseSelectionPopulationConcentration:
            if ( !GetPopulationConcentrationAtPos( pPopulationConcentration, vGLPos ) && pPopulationConcentration )
            {
                selectedElement_ = MOS_SelectedElement( *pPopulationConcentration );
                return;
            }
            nSelectionState = ePhaseSelectionPopulationFlow;
            break;
        case ePhaseSelectionPopulationFlow:
            if ( !GetPopulationFlowAtPos( pPopulationFlow, vGLPos, rDistancePerPixel ) && pPopulationFlow )
            {
                selectedElement_ = MOS_SelectedElement( *pPopulationFlow );
                return;
            }
            nSelectionState = ePhaseSelectionLooping;
            break;
        case ePhaseSelectionLooping:
            nSelectionState = ePhaseSelectionAgent;
            bLoopDone = true;
            break;
        }
    }
    //if nothing has been selected: select a point...
    if( selectedElement_.pLine_ != 0 )
    {
        int nPoint = GetPointAtPos( *selectedElement_.pLine_, vGLPos, rDistancePerPixel );
        if( nPoint != -1 )
        {
            selectedElement_ = MOS_SelectedElement( *selectedElement_.pLine_, nPoint );
            return;
        }
    }
    //... or a tactical line
    if( MOS_MainWindow::GetMainWindow().GetOptions().nDrawTacticalLines_ != MOS_Options::eOff )
    {
        MOS_TacticalLine_ABC* pLine = GetLineAtPos( vGLPos, rDistancePerPixel );
        if( pLine != 0 )
        {
            selectedElement_ = MOS_SelectedElement( *pLine );
            return;
        }
    }

    selectedElement_ = MOS_SelectedElement();   
}



// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetPopulationAtPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
MOS_Population* MOS_DefaultMapEventHandler::GetPopulationAtPos( const MT_Vector2D& vGLPos )
{
    // $$$$ SBO 2005-10-26: To be removed as we cannot select a full population
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetPopulationConcentrationAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetPopulationConcentrationAtPos( MOS_PopulationConcentration*& pPopulationConcentrationReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false;
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    MOS_AgentManager::CT_PopulationMap& populationMap = MOS_App::GetApp().GetAgentManager().GetPopulationList();
    if( ! populationMap.size() )
        return false;

    MOS_AgentManager::RCIT_PopulationMap rit;
    MOS_Population::RCIT_ConcentrationMap ritConcentration;

    if( selectedElement_.pPopulationConcentration_ == 0 )
    {
        rit = populationMap.rbegin();
        assert( rit->second );
        ritConcentration = rit->second->GetConcentrations().rbegin();
    }
    else
    {
        // get the next concentration from population
        MOS_AgentManager::CIT_PopulationMap it = populationMap.find( selectedElement_.pPopulationConcentration_->GetPopulation().GetID() );
        assert( it != populationMap.end() );
        rit = MOS_AgentManager::RCIT_PopulationMap( it );
        MOS_Population::CIT_ConcentrationMap itConcentration = rit->second->GetConcentrations().find( selectedElement_.pPopulationConcentration_->GetID() );
        ritConcentration = MOS_Population::RCIT_ConcentrationMap( ++itConcentration );
    }

    for( uint nPopulation = 0; nPopulation < populationMap.size(); ++nPopulation )
    {
        if( rit == populationMap.rend() )
        {
            bRescan = true;
            rit = populationMap.rbegin();
        }
        const MOS_Population* pPopulation = (*rit).second;
        if( nPopulation > 0 )
            ritConcentration = pPopulation->GetConcentrations().rbegin();
        for( uint nConcentration = 0; nConcentration < pPopulation->GetConcentrations().size(); ++nConcentration )
        {
            if( ritConcentration == pPopulation->GetConcentrations().rend() )
                break;
            MOS_PopulationConcentration* pConcentration = (*ritConcentration).second;
            if( nPlayedTeam == MOS_Options::eController || nPlayedTeam == (int)(pPopulation->GetTeam().GetIdx()) )
            {
                if( IsPopulationConcentrationAtPos( *pConcentration, vGLPos ) )
                {
                    pPopulationConcentrationReturned = pConcentration;
                    return bRescan;
                }
            }
            ++ritConcentration;
        }
        ++rit;
    }
    return bRescan;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetPopulationFlowAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetPopulationFlowAtPos( MOS_PopulationFlow*& pPopulationFlowReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false;
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    MOS_AgentManager::CT_PopulationMap& populationMap = MOS_App::GetApp().GetAgentManager().GetPopulationList();
    if( ! populationMap.size() )
        return false;

    MOS_AgentManager::RCIT_PopulationMap rit;
    MOS_Population::RCIT_FlowMap ritFlow;

    if( selectedElement_.pPopulationFlow_ == 0 )
    {
        rit = populationMap.rbegin();
        assert( rit->second );
        ritFlow = rit->second->GetFlows().rbegin();
    }
    else
    {
        // get the next concentration from population
        MOS_AgentManager::CIT_PopulationMap it = populationMap.find( selectedElement_.pPopulationFlow_->GetPopulation().GetID() );
        assert( it != populationMap.end() );
        rit = MOS_AgentManager::RCIT_PopulationMap( it );
        MOS_Population::CIT_FlowMap itFlow = rit->second->GetFlows().find( selectedElement_.pPopulationFlow_->GetID() );
        ritFlow = MOS_Population::RCIT_FlowMap( ++itFlow );
    }

    for( uint nPopulation = 0; nPopulation < populationMap.size(); ++nPopulation )
    {
        if( rit == populationMap.rend() )
        {
            bRescan = true;
            rit = populationMap.rbegin();
        }
        const MOS_Population* pPopulation = (*rit).second;
        if( nPopulation > 0 )
            ritFlow = pPopulation->GetFlows().rbegin();
        for( uint nFlow = 0; nFlow < pPopulation->GetFlows().size(); ++nFlow )
        {
            if( ritFlow == pPopulation->GetFlows().rend() )
                break;
            MOS_PopulationFlow* pFlow = (*ritFlow).second;
            if( nPlayedTeam == MOS_Options::eController || nPlayedTeam == (int)(pPopulation->GetTeam().GetIdx()) )
            {
                if( IsPopulationFlowAtPos( *pFlow, vGLPos, rDistancePerPixel ) )
                {
                    pPopulationFlowReturned = pFlow;
                    return bRescan;
                }
            }
            ++ritFlow;
        }
        ++rit;
    }
    return bRescan;
}

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsPopulationConcentrationAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsPopulationConcentrationAtPos( const MOS_PopulationConcentration& concentration, const MT_Vector2D& vGLPos ) const
{
    MT_Float rSurface = concentration.GetLivingHumans() / concentration.GetPopulation().GetType().GetConcentrationDensity();
    MT_Circle concentrationCircle( concentration.GetPos().rX_, concentration.GetPos().rY_, std::sqrt( rSurface / MT_PI ) );
    return concentrationCircle.Inside( vGLPos );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsPopulationFlowAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsPopulationFlowAtPos( const MOS_PopulationFlow& flow, const MT_Vector2D& vGLPos, float rDistancePerPixel ) const
{
    return MOS_Tools::PointNearLine( vGLPos, flow.GetFlow(), 9.0 * rDistancePerPixel );
}

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::HoverElementAtPos
// Created: HME 2005-11-24
// -----------------------------------------------------------------------------
void MOS_DefaultMapEventHandler::HoverElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_Agent* pAgent = 0;
    GetAgentAtPos( pAgent, vGLPos );
    if( pAgent )
    {
        hoveredElement_ = MOS_SelectedElement( *pAgent );
        emit ElementHovered( hoveredElement_ );
        return;
    }  
    if ( hoveredElement_.IsAMapElementSelected() )
    {
        hoveredElement_ = MOS_SelectedElement();
        emit ElementHovered( hoveredElement_ );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetAgentAtPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetAgentAtPos( MOS_Agent*& pAgentReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false;
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    MOS_AgentManager::CT_AgentMap& agentMap = MOS_App::GetApp().GetAgentManager().GetAgentList();
    if( ! agentMap.size() )
        return 0;

    MOS_AgentManager::RCIT_AgentMap rit;

    if( selectedElement_.pAgent_ == 0 )
    {
        rit = agentMap.rbegin();
    }
    else
    {
        MOS_AgentManager::CIT_AgentMap it = agentMap.find( selectedElement_.pAgent_->GetID() );
        assert( it != agentMap.end() );
        rit = MOS_AgentManager::RCIT_AgentMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < agentMap.size(); ++n )
    {
        if( rit == agentMap.rend() )
        {
            bRescan = true;
            rit = agentMap.rbegin();
        }

        MOS_Agent* pAgent = (*rit).second;
        if( nPlayedTeam == MOS_Options::eController || nPlayedTeam == (int)(pAgent->GetTeam().GetIdx()) )
        {
            if( IsAgentAtPos( *pAgent, vGLPos ) )
            {
                pAgentReturned = pAgent;
                return bRescan;
            }
        }

        ++rit;
    }
    pAgentReturned = 0;
    return bRescan;
}

// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetAgentKnowledgeAtPos
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetAgentKnowledgeAtPos( MOS_AgentKnowledge*& pAgentKnowledgeReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false; 
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    assert( nPlayedTeam != MOS_Options::eController );  // You can't select agent knowledges on the map when in 'all teams' mode (ie. controller view).

    MOS_Team* pPlayedTeam = MOS_App::GetApp().GetAgentManager().FindTeamFromIdx( nPlayedTeam );
    assert( pPlayedTeam != 0 );

    const MOS_Team::T_AgentKnowledgeMap& agentMap = pPlayedTeam->GetAgentKnowledges();
    if( ! agentMap.size() )
        return 0;

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
        {
            bRescan = true;
            rit = agentMap.rbegin();
        }

        MOS_AgentKnowledge* pAgentKnowledge = (*rit).second;
        // Only allow the selection of agent knowledges pointing to agents not in our team.
        if( pAgentKnowledge->GetRealAgent().GetTeam().GetID() != pPlayedTeam->GetID() )
        {
            if( IsAgentKnowledgeAtPos( *pAgentKnowledge, vGLPos ) )
            {
                pAgentKnowledgeReturned = pAgentKnowledge;
                return bRescan;
            }
        }

        ++rit;
    }

    return bRescan;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetLineAtPos
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
MOS_TacticalLine_ABC* MOS_DefaultMapEventHandler::GetLineAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    MOS_LineManager::CT_TacticalLineSet& lineSet = MOS_App::GetApp().GetLineManager().GetLineList();
    if( ! lineSet.size() )
        return 0;

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
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetObjectAtPos( MOS_Object_ABC*&  pObjectReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false;
    const MOS_ObjectManager::T_ObjectMap& objectMap = MOS_App::GetApp().GetObjectManager().GetObjects();
    if( ! objectMap.size() )
        return 0;

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
        {
            bRescan = true;
            rit = objectMap.rbegin();
        }

        MOS_Object_ABC* pObject = (*rit).second;
        if( IsObjectAtPos( *pObject, vGLPos, rDistancePerPixel ) )
        {
            pObjectReturned = pObject;
            return bRescan;
        }

        ++rit;
    }
    pObjectReturned = 0;
    return bRescan;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsObjectAtPos
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::IsObjectAtPos( const MOS_Object_ABC& object, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    return MOS_Tools::PointNearLine( vGLPos, object.GetPointList(), 9.0 * rDistancePerPixel, object.GetLocationType() == EnumTypeLocalisation::polygon );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetObjectKnowledgeAtPos
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
bool MOS_DefaultMapEventHandler::GetObjectKnowledgeAtPos( MOS_ObjectKnowledge*& pObjectKnowledgeReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false; 
    MOS_Options& options = MOS_MainWindow::GetMainWindow().GetOptions();
    assert( options.nPlayedTeam_ != MOS_Options::eController );
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeamFromIdx( options.nPlayedTeam_ );
    assert( pTeam != 0 );
    const MOS_Team::T_ObjectKnowledgeMap& objectMap = pTeam->GetObjectKnowledges();
    if( ! objectMap.size() )
        return 0;

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
        {
            bRescan = true;           
            rit = objectMap.rbegin();
        }

        MOS_ObjectKnowledge* pObject = (*rit).second;
        if( IsObjectKnowledgeAtPos( *pObject, vGLPos, rDistancePerPixel ) )
        {
            pObjectKnowledgeReturned = pObject;
            return bRescan;
        }

        ++rit;
    }

    return bRescan;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsObjectKnowledgeAtPos
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
       || ( selectedElement_.pAgent_                   != 0 && ! IsAgentAtPos                  ( *selectedElement_.pAgent_, vGLPos ) )
       //|| ( selectedElement_.pPopulation_          != 0 && ! IsAgentAtPos              ( *selectedElement_.pPopulation_, vGLPos ) )
       || ( selectedElement_.pPopulationConcentration_ != 0 && ! IsPopulationConcentrationAtPos( *selectedElement_.pPopulationConcentration_, vGLPos ) )
       || ( selectedElement_.pPopulationFlow_          != 0 && ! IsPopulationFlowAtPos         ( *selectedElement_.pPopulationFlow_, vGLPos, rDistancePerPixel ) )
       || ( selectedElement_.pObject_                  != 0 && ! IsObjectAtPos                 ( *selectedElement_.pObject_, vGLPos, rDistancePerPixel ) )
       || ( selectedElement_.pLine_                    != 0 && ! IsLineAtPos                   ( *selectedElement_.pLine_, vGLPos, rDistancePerPixel ) )
       || ( selectedElement_.pAgentKnowledge_          != 0 && ! IsAgentKnowledgeAtPos         ( *selectedElement_.pAgentKnowledge_, vGLPos ) ) 
       || ( selectedElement_.pObjectKnowledge_         != 0 && ! IsObjectKnowledgeAtPos        ( *selectedElement_.pObjectKnowledge_, vGLPos, rDistancePerPixel ) ) )
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
