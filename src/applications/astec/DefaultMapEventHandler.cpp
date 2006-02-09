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
// $Archive: /MVW_v10/Build/SDK/Light2/src/DefaultMapEventHandler.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:49 $
// $Revision: 14 $
// $Workfile: DefaultMapEventHandler.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "DefaultMapEventHandler.h"
#include "moc_DefaultMapEventHandler.cpp"

#include "App.h"
#include "MainWindow.h"
#include "ShapeEditorMapEventFilter.h"
#include "AgentManager.h"
#include "Object_ABC.h"
#include "ObjectKnowledge.h"
#include "ObjectManager.h"
#include "ActionContext.h"
#include "TacticalLine_ABC.h"
#include "LineManager.h"
#include "Limit.h"
#include "Lima.h"
#include "Tools.h"
#include "GLTool.h"
#include "Options.h"
#include "Team.h"
#include "AgentKnowledge.h"
#include "MapMouseEvent.h"
#include "ASN_Messages.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"

#include "MT_Tools/MT_Circle.h"


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler constructor
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
DefaultMapEventHandler::DefaultMapEventHandler( QObject* pParent )
    : QObject                ( pParent )
    , MapEventFilter_ABC ()
    , selectedElement_       ()
    , pPopupMenu_            ( 0 )
    , nSelectionState         ( ePhaseSelectionAgent )
{
    pLineEditor_ = new ShapeEditorMapEventFilter( this );
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( LineCreated() ) );

    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),               &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );
    connect( this, SIGNAL( ElementHovered( SelectedElement& ) ),                 &MainWindow::GetMainWindow(), SIGNAL( ElementHovered( SelectedElement& ) ) );

    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( ClearSelection() ) );

    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( ClearSelection() ) );

    connect( &App::GetApp(), SIGNAL( ObjectDeleted( Object_ABC& ) ), this, SLOT( OnObjectDeleted( Object_ABC& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationConcentrationDeleted( const PopulationConcentration& ) ), this, SLOT( OnPopulationConcentrationDeleted( const PopulationConcentration& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationFlowDeleted( const PopulationFlow& ) ), this, SLOT( OnPopulationFlowDeleted( const PopulationFlow& ) ) );
    connect( &App::GetApp(), SIGNAL( PopulationDeleted( Population& ) ), this, SLOT( OnPopulationDeleted( Population& ) ) );
    connect( &App::GetApp(), SIGNAL( TacticalLineDeleted( TacticalLine_ABC& ) ), this, SLOT( OnTacticalLineDeleted( TacticalLine_ABC& ) ) );
    connect( &App::GetApp(), SIGNAL( AgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ), this, SLOT( OnAgentKnowledgeDeleted( Gtia&, AgentKnowledge& ) ) );
    connect( &App::GetApp(), SIGNAL( ObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ), this, SLOT( OnObjectKnowledgeDeleted( Team&, ObjectKnowledge& ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );

}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler destructor
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
DefaultMapEventHandler::~DefaultMapEventHandler()
{
    // Deletion order is important, as both the pLineEditor and this can pop themselves off
    // the handler list. The pLineEditor must be popped first.
    delete pLineEditor_;

    delete pPopupMenu_;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::Draw
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::Draw() const
{
    GLTool::Draw( *this );
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::eventFilter
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::KeyPress:
            return OnKeyPress( *(QKeyEvent*)pEvent );

        // All QMouseEvent are actually MapMouseEvent. Cast and use them.
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick:
            return OnMousePress( *(MapMouseEvent*)pEvent );
            
        case QEvent::MouseMove:
            return OnMouseMove( *(MapMouseEvent*)pEvent );

        case QEvent::MouseButtonRelease:
            return OnMouseRelease( *(MapMouseEvent*)pEvent );
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnMousePress
// Created: APE 2004-03-26
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::OnMousePress( const MapMouseEvent& mouseEvent )
{
    vOldMousePos_ = mouseEvent.GetMapPos();
    bool bAutomata = ( mouseEvent.state() == ShiftButton || mouseEvent.state() == AltButton ) && ( ! App::GetApp().IsODBEdition() );
    if( mouseEvent.button() == LeftButton && ( mouseEvent.state() ==  NoButton || mouseEvent.state() == ShiftButton || mouseEvent.state() == AltButton ) )
    {
        SelectElementAtPos( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel(), bAutomata );
        //if needed center on the automata
        if( selectedElement_.pAgent_ && bAutomata && mouseEvent.state() == AltButton )
        {
            emit CenterOnPoint( selectedElement_.pAgent_->GetPos() );
        }
        //connect and connect to avoid having the signal come back to us.
        disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
        emit ElementSelected( selectedElement_ );
        connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
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
// Name: DefaultMapEventHandler::OnMouseRelease
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::OnMouseRelease( const MapMouseEvent& mouseEvent )
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
// Name: DefaultMapEventHandler::OnMouseMove
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::OnMouseMove( const MapMouseEvent& mouseEvent )
{
    // If moving a tactical line's point around...
    if( mouseEvent.state() == (LeftButton | ShiftButton) )
    {
        if(  selectedElement_.nLinePoint_ != -1 
        && (! selectedElement_.pLine_->IsUpdatingToSim() || App::GetApp().IsODBEdition() ) )
        {
            selectedElement_.pLine_->ModifyPoint( selectedElement_.nLinePoint_, mouseEvent.GetMapPos() );
            vOldMousePos_ = mouseEvent.GetMapPos();
            return true;
        }

    // If moving a tactical line around...
        if(      selectedElement_.pLine_ != 0 
        &&   selectedElement_.nLinePoint_ == -1 
        && (! selectedElement_.pLine_->IsUpdatingToSim() || App::GetApp().IsODBEdition() ) )
        {
           selectedElement_.pLine_->Translate( mouseEvent.GetMapPos() - vOldMousePos_ );
           vOldMousePos_ = mouseEvent.GetMapPos();
           return true;
        }
    }
    
    //deal with over events, the test is only here for performance
    if( MainWindow::GetMainWindow().GetOptions().bDisplayHoveredInfo_ )
        HoverElementAtPos( mouseEvent.GetMapPos(), mouseEvent.GetDistancePerPixel() ) ;
    
    vOldMousePos_ = mouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnKeyPress
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::OnKeyPress( const QKeyEvent& keyEvent )
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
                selectedElement_.pLine_->SetState( TacticalLine_ABC::eStateDeleted );
                selectedElement_.pLine_->UpdateToSim();
                return true;
            }
            
            // If we're trying to delete a line when editing an odb, delete it
            // immediatly.
            if( selectedElement_.pLine_ != 0  && App::GetApp().IsODBEdition() )
            {
                App::GetApp().GetLineManager().DeleteLine( selectedElement_.pLine_->GetID() );
                selectedElement_.pLine_ = 0;
                selectedElement_.nLinePoint_ = -1;
                return true;
            }
        }
    }
    return false;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::SwitchToLineCreation
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::SwitchToLineCreation( int nType )
{
    nLineType_ = nType;
    pLineEditor_->PrepareNewLine( ShapeEditorMapEventFilter::eTacticalLine );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::LineCreated
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::LineCreated()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.size() < 2 )
        return;

    if( nLineType_ == 999 )
    {
        // It's a limit.
        Limit* pLimit = new Limit( pointList );
        App::GetApp().GetLineManager().RegisterLine( *pLimit );
        if( ! App::GetApp().IsODBEdition() )
            pLimit->UpdateToSim();
    }
    else
    {
        // It's a lima, and nLineType_ is the lima's type.
        Lima* pLima = new Lima( pointList, (E_FuncLimaType)nLineType_ );
        App::GetApp().GetLineManager().RegisterLine( *pLima );
        if( ! App::GetApp().IsODBEdition() )
            pLima->UpdateToSim();
    }
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::ClearSelection
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::ClearSelection()
{
    selectedElement_ = SelectedElement();
    emit ElementSelected( selectedElement_ );
    hoveredElement_  = SelectedElement();
    emit ElementHovered ( hoveredElement_ );
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnObjectDeleted
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnObjectDeleted( Object_ABC& object )
{
    if( selectedElement_.pObject_ == &object )
    {
        selectedElement_ = SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnPopulationConcentrationDeleted
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnPopulationConcentrationDeleted( const PopulationConcentration& concentration )
{
    if( selectedElement_.pPopulationConcentration_ == &concentration )
    {
        selectedElement_ = SelectedElement();
        selectedElement_.pPopulation_ = const_cast< Population* >( &concentration.GetPopulation() );
        emit ElementSelected( selectedElement_ );
    }
}
    
// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnPopulationFlowDeleted
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnPopulationFlowDeleted( const PopulationFlow& flow )
{
    if( selectedElement_.pPopulationFlow_ == &flow )
    {
        selectedElement_ = SelectedElement();
        selectedElement_.pPopulation_ = const_cast< Population* >( &flow.GetPopulation() );
        emit ElementSelected( selectedElement_ );
    }    
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnTacticalLineDeleted
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnTacticalLineDeleted( TacticalLine_ABC& line )
{
    if( selectedElement_.pLine_ == &line )
    {
        selectedElement_ = SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnAgentKnowledgeDeleted
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnAgentKnowledgeDeleted( Gtia& /*gtia*/, AgentKnowledge& knowledge )
{
    if( selectedElement_.pAgentKnowledge_ == &knowledge )
    {
        selectedElement_ = SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnObjectKnowledgeDeleted
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnObjectKnowledgeDeleted( Team& /*team*/, ObjectKnowledge& knowledge )
{
    if( selectedElement_.pObjectKnowledge_ == &knowledge )
    {
        selectedElement_ = SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::OnPopulationDeleted
// Created: HME 2006-01-03
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::OnPopulationDeleted( Population& pop )
{
    if( selectedElement_.pPopulation_ == &pop )
    {
        selectedElement_ = SelectedElement();
        emit ElementSelected( selectedElement_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::SelectElementAtPos
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::SelectElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel , bool bAutomata )
{
    //state machine for selection of things, the selection will loop over all overlapped things
    int nBeginningState = nSelectionState;
    bool bLoopDone = false;
    Agent* pAgent = 0;
    Object_ABC* pObject = 0;
    ObjectKnowledge* pObjectKnowledge = 0;
    AgentKnowledge* pAgentKnowledge = 0;
    PopulationConcentration* pPopulationConcentration = 0;
    PopulationFlow* pPopulationFlow = 0;
    //search while a whole loop over the overlapped things has been done
    while ( nSelectionState != nBeginningState || ! bLoopDone )
    {
        switch( nSelectionState )
        {
        case ePhaseSelectionAgent:
            if ( !GetAgentAtPos( pAgent, vGLPos ) && pAgent )
            {
                if( bAutomata && ( ! pAgent->IsAutomate() ) )
                    selectedElement_ = SelectedElement( *pAgent->GetParent() );
                else
                    selectedElement_ = SelectedElement( *pAgent );
                return;
            }
            nSelectionState = ePhaseSelectionObject;
            break;
        case ePhaseSelectionObject:
            if( MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != Options::eController )
            {
                nSelectionState = ePhaseSelectionAgentKnowledge;
                break;
            }
            if ( ! GetObjectAtPos( pObject, vGLPos, rDistancePerPixel ) && pObject )
            {
                selectedElement_ = SelectedElement( *pObject );
                return;
            }
            nSelectionState = ePhaseSelectionAgentKnowledge;;
            break;
        case ePhaseSelectionAgentKnowledge:
            if( MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ == Options::eController )
            {
                nSelectionState = ePhaseSelectionObjectKnowledge;
                break;
            }
            if ( !GetAgentKnowledgeAtPos( pAgentKnowledge, vGLPos ) && pAgentKnowledge )
            {
                selectedElement_ = SelectedElement( *pAgentKnowledge );
                return;
            }
            nSelectionState = ePhaseSelectionObjectKnowledge;
            break;
        case ePhaseSelectionObjectKnowledge:
            if( MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ == Options::eController )
            {
                nSelectionState = ePhaseSelectionPopulationConcentration;
                break;
            }
            if ( !GetObjectKnowledgeAtPos( pObjectKnowledge, vGLPos, rDistancePerPixel ) && pObjectKnowledge )
            {
                selectedElement_ = SelectedElement( *pObjectKnowledge );
                return;
            }
            nSelectionState = ePhaseSelectionPopulationConcentration;
            break;
        case ePhaseSelectionPopulationConcentration:
            if ( !GetPopulationConcentrationAtPos( pPopulationConcentration, vGLPos ) && pPopulationConcentration )
            {
                selectedElement_ = SelectedElement( *pPopulationConcentration );
                return;
            }
            nSelectionState = ePhaseSelectionPopulationFlow;
            break;
        case ePhaseSelectionPopulationFlow:
            if ( !GetPopulationFlowAtPos( pPopulationFlow, vGLPos, rDistancePerPixel ) && pPopulationFlow )
            {
                selectedElement_ = SelectedElement( *pPopulationFlow );
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
            selectedElement_ = SelectedElement( *selectedElement_.pLine_, nPoint );
            return;
        }
    }
    //... or a tactical line
    if( MainWindow::GetMainWindow().GetOptions().nDrawTacticalLines_ != Options::eOff )
    {
        TacticalLine_ABC* pLine = GetLineAtPos( vGLPos, rDistancePerPixel );
        if( pLine != 0 )
        {
            selectedElement_ = SelectedElement( *pLine );
            return;
        }
    }

    selectedElement_ = SelectedElement();   
}



// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetPopulationAtPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
Population* DefaultMapEventHandler::GetPopulationAtPos( const MT_Vector2D& vGLPos )
{
    // $$$$ SBO 2005-10-26: To be removed as we cannot select a full population
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetPopulationConcentrationAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetPopulationConcentrationAtPos( PopulationConcentration*& pPopulationConcentrationReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false;
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    AgentManager::CT_PopulationMap& populationMap = App::GetApp().GetAgentManager().GetPopulationList();
    if( ! populationMap.size() )
        return false;

    AgentManager::RCIT_PopulationMap rit;
    Population::RCIT_ConcentrationMap ritConcentration;

    if( selectedElement_.pPopulationConcentration_ == 0 )
    {
        rit = populationMap.rbegin();
        assert( rit->second );
        ritConcentration = rit->second->GetConcentrations().rbegin();
    }
    else
    {
        // get the next concentration from population
        AgentManager::CIT_PopulationMap it = populationMap.find( selectedElement_.pPopulationConcentration_->GetPopulation().GetID() );
        assert( it != populationMap.end() );
        rit = AgentManager::RCIT_PopulationMap( it );
        Population::CIT_ConcentrationMap itConcentration = rit->second->GetConcentrations().find( selectedElement_.pPopulationConcentration_->GetID() );
        ritConcentration = Population::RCIT_ConcentrationMap( ++itConcentration );
    }

    for( uint nPopulation = 0; nPopulation < populationMap.size(); ++nPopulation )
    {
        if( rit == populationMap.rend() )
        {
            bRescan = true;
            rit = populationMap.rbegin();
        }
        const Population* pPopulation = (*rit).second;
        if( nPopulation > 0 )
            ritConcentration = pPopulation->GetConcentrations().rbegin();
        for( uint nConcentration = 0; nConcentration < pPopulation->GetConcentrations().size(); ++nConcentration )
        {
            if( ritConcentration == pPopulation->GetConcentrations().rend() )
                break;
            PopulationConcentration* pConcentration = (*ritConcentration).second;
            if( nPlayedTeam == Options::eController || nPlayedTeam == (int)(pPopulation->GetTeam().GetIdx()) )
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
// Name: DefaultMapEventHandler::GetPopulationFlowAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetPopulationFlowAtPos( PopulationFlow*& pPopulationFlowReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false;
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    AgentManager::CT_PopulationMap& populationMap = App::GetApp().GetAgentManager().GetPopulationList();
    if( ! populationMap.size() )
        return false;

    AgentManager::RCIT_PopulationMap rit;
    Population::RCIT_FlowMap ritFlow;

    if( selectedElement_.pPopulationFlow_ == 0 )
    {
        rit = populationMap.rbegin();
        assert( rit->second );
        ritFlow = rit->second->GetFlows().rbegin();
    }
    else
    {
        // get the next concentration from population
        AgentManager::CIT_PopulationMap it = populationMap.find( selectedElement_.pPopulationFlow_->GetPopulation().GetID() );
        assert( it != populationMap.end() );
        rit = AgentManager::RCIT_PopulationMap( it );
        Population::CIT_FlowMap itFlow = rit->second->GetFlows().find( selectedElement_.pPopulationFlow_->GetID() );
        ritFlow = Population::RCIT_FlowMap( ++itFlow );
    }

    for( uint nPopulation = 0; nPopulation < populationMap.size(); ++nPopulation )
    {
        if( rit == populationMap.rend() )
        {
            bRescan = true;
            rit = populationMap.rbegin();
        }
        const Population* pPopulation = (*rit).second;
        if( nPopulation > 0 )
            ritFlow = pPopulation->GetFlows().rbegin();
        for( uint nFlow = 0; nFlow < pPopulation->GetFlows().size(); ++nFlow )
        {
            if( ritFlow == pPopulation->GetFlows().rend() )
                break;
            PopulationFlow* pFlow = (*ritFlow).second;
            if( nPlayedTeam == Options::eController || nPlayedTeam == (int)(pPopulation->GetTeam().GetIdx()) )
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
// Name: DefaultMapEventHandler::IsPopulationConcentrationAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::IsPopulationConcentrationAtPos( const PopulationConcentration& concentration, const MT_Vector2D& vGLPos ) const
{
    MT_Float rSurface = concentration.GetLivingHumans() / concentration.GetPopulation().GetType().GetConcentrationDensity();
    MT_Circle concentrationCircle( concentration.GetPos().rX_, concentration.GetPos().rY_, std::sqrt( rSurface / MT_PI ) );
    return concentrationCircle.Inside( vGLPos );
}
    
// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::IsPopulationFlowAtPos
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::IsPopulationFlowAtPos( const PopulationFlow& flow, const MT_Vector2D& vGLPos, float rDistancePerPixel ) const
{
    return Tools::PointNearLine( vGLPos, flow.GetFlow(), 9.0 * rDistancePerPixel );
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::HoverElementAtPos
// Created: HME 2005-11-24
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::HoverElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{

    if( MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ != Options::eController )
    {
        AgentKnowledge* pAgentKnowledge = 0;
        GetAgentKnowledgeAtPos( pAgentKnowledge, vGLPos );
        if( pAgentKnowledge )
        {
            hoveredElement_ = SelectedElement( *pAgentKnowledge );
            emit ElementHovered( hoveredElement_ );
            return;
        }
        ObjectKnowledge* pObjectKnowledge = 0;
        GetObjectKnowledgeAtPos( pObjectKnowledge, vGLPos, rDistancePerPixel );
        if( pObjectKnowledge )
        {
            hoveredElement_ = SelectedElement( *pObjectKnowledge );
            emit ElementHovered( hoveredElement_ );
            return;
        }
    }

    Agent* pAgent = 0;
    GetAgentAtPos( pAgent, vGLPos );
    if( pAgent && ( pAgent->IsAutomate() || ! pAgent->IsAggregated() ) )
    {
        hoveredElement_ = SelectedElement( *pAgent );
        emit ElementHovered( hoveredElement_ );
        return;
    }
    PopulationConcentration* pPopulationConcentration = 0;
    GetPopulationConcentrationAtPos( pPopulationConcentration, vGLPos );
    if( pPopulationConcentration )
    {
        hoveredElement_ = SelectedElement( *pPopulationConcentration );
        emit ElementHovered( hoveredElement_ );
        return;
    }
    PopulationFlow* pPopulationFlow = 0;
    GetPopulationFlowAtPos( pPopulationFlow, vGLPos, rDistancePerPixel );
    if( pPopulationFlow )
    {
        hoveredElement_ = SelectedElement( *pPopulationFlow );
        emit ElementHovered( hoveredElement_ );
        return;
    }
    Object_ABC* pObject = 0;
    GetObjectAtPos( pObject, vGLPos, rDistancePerPixel );
    if( pObject )
    {
        hoveredElement_ = SelectedElement( *pObject );
        emit ElementHovered( hoveredElement_ );
        return;
    }

    //If nothing is overed
    if ( hoveredElement_.IsAMapElementSelected() )
    {
        hoveredElement_ = SelectedElement();
        emit ElementHovered( hoveredElement_ );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetAgentAtPos
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetAgentAtPos( Agent*& pAgentReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false;
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;

    AgentManager::CT_AgentMap& agentMap = App::GetApp().GetAgentManager().GetAgentList();
    if( ! agentMap.size() )
        return 0;

    AgentManager::RCIT_AgentMap rit;

    if( selectedElement_.pAgent_ == 0 )
    {
        rit = agentMap.rbegin();
    }
    else
    {
        AgentManager::CIT_AgentMap it = agentMap.find( selectedElement_.pAgent_->GetID() );
        assert( it != agentMap.end() );
        rit = AgentManager::RCIT_AgentMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < agentMap.size(); ++n )
    {
        if( rit == agentMap.rend() )
        {
            bRescan = true;
            rit = agentMap.rbegin();
        }

        Agent* pAgent = (*rit).second;
        if( nPlayedTeam == Options::eController || nPlayedTeam == (int)(pAgent->GetTeam().GetIdx()) )
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
// Name: DefaultMapEventHandler::GetAgentKnowledgeAtPos
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetAgentKnowledgeAtPos( AgentKnowledge*& pAgentKnowledgeReturned, const MT_Vector2D& vGLPos )
{
    bool bRescan = false; 
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    assert( nPlayedTeam != Options::eController );  // You can't select agent knowledges on the map when in 'all teams' mode (ie. controller view).

    Team* pPlayedTeam = App::GetApp().GetAgentManager().FindTeamFromIdx( nPlayedTeam );
    assert( pPlayedTeam != 0 );

    const Team::T_AgentKnowledgeMap& agentMap = pPlayedTeam->GetAgentKnowledges();
    if( ! agentMap.size() )
        return 0;

    Team::RCIT_AgentKnowledgeMap rit;

    if( selectedElement_.pAgentKnowledge_ == 0 )
    {
        rit = agentMap.rbegin();
    }
    else
    {
        Team::CIT_AgentKnowledgeMap it = agentMap.find( selectedElement_.pAgentKnowledge_->GetID() );
        assert( it != agentMap.end() );
        rit = Team::RCIT_AgentKnowledgeMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < agentMap.size(); ++n )
    {
        if( rit == agentMap.rend() )
        {
            bRescan = true;
            rit = agentMap.rbegin();
        }

        AgentKnowledge* pAgentKnowledge = (*rit).second;
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
// Name: DefaultMapEventHandler::GetLineAtPos
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
TacticalLine_ABC* DefaultMapEventHandler::GetLineAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    LineManager::CT_TacticalLineSet& lineSet = App::GetApp().GetLineManager().GetLineList();
    if( ! lineSet.size() )
        return 0;

    LineManager::RCIT_TacticalLineSet rit;

    if( selectedElement_.pLine_ == 0 )
    {
        rit = lineSet.rbegin();
    }
    else
    {
        LineManager::CIT_TacticalLineSet it = lineSet.find( selectedElement_.pLine_ );
        assert( it != lineSet.end() );
        rit = LineManager::RCIT_TacticalLineSet( ++it );
        ++rit;
    }

    for( uint n = 0; n < lineSet.size(); ++n )
    {
        if( rit == lineSet.rend() )
            rit = lineSet.rbegin();

        TacticalLine_ABC* pLine = (*rit);
        if( IsLineAtPos( *pLine, vGLPos, rDistancePerPixel ) )
            return pLine;

        ++rit;
    }

    return 0;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::IsLineAtPos
// Created: APE 2004-04-14
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::IsLineAtPos( const TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    CT_PointVector& pointList = line.GetPointList();
    assert( pointList.size() >= 2 );

    return Tools::PointNearLine( vGLPos, pointList, 9.0 * rDistancePerPixel );
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetPointAtPos
// Created: APE 2004-04-16
// -----------------------------------------------------------------------------
int DefaultMapEventHandler::GetPointAtPos( TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel )
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
// Name: DefaultMapEventHandler::GetObjectAt
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetObjectAtPos( Object_ABC*&  pObjectReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false;
    const ObjectManager::T_ObjectMap& objectMap = App::GetApp().GetObjectManager().GetObjects();
    if( ! objectMap.size() )
        return 0;

    ObjectManager::RCIT_ObjectMap rit;

    if( selectedElement_.pObject_ == 0 )
    {
        rit = objectMap.rbegin();
    }
    else
    {
        ObjectManager::CIT_ObjectMap it = objectMap.find( selectedElement_.pObject_->GetID() );
        assert( it != objectMap.end() );
        rit = ObjectManager::RCIT_ObjectMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < objectMap.size(); ++n )
    {
        if( rit == objectMap.rend() )
        {
            bRescan = true;
            rit = objectMap.rbegin();
        }

        Object_ABC* pObject = (*rit).second;
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
// Name: DefaultMapEventHandler::IsObjectAtPos
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::IsObjectAtPos( const Object_ABC& object, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    return Tools::PointNearLine( vGLPos, object.GetPointList(), 9.0 * rDistancePerPixel, object.GetLocationType() == EnumTypeLocalisation::polygon );
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetObjectKnowledgeAtPos
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::GetObjectKnowledgeAtPos( ObjectKnowledge*& pObjectKnowledgeReturned, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    bool bRescan = false; 
    Options& options = MainWindow::GetMainWindow().GetOptions();
    assert( options.nPlayedTeam_ != Options::eController );
    Team* pTeam = App::GetApp().GetAgentManager().FindTeamFromIdx( options.nPlayedTeam_ );
    assert( pTeam != 0 );
    const Team::T_ObjectKnowledgeMap& objectMap = pTeam->GetObjectKnowledges();
    if( ! objectMap.size() )
        return 0;

    Team::RCIT_ObjectKnowledgeMap rit;

    if( selectedElement_.pObjectKnowledge_ == 0 )
    {
        rit = objectMap.rbegin();
    }
    else
    {
        Team::CIT_ObjectKnowledgeMap it = objectMap.find( selectedElement_.pObjectKnowledge_->GetID() );
        assert( it != objectMap.end() );
        rit = Team::RCIT_ObjectKnowledgeMap( ++it );
        ++rit;
    }

    for( uint n = 0; n < objectMap.size(); ++n )
    {
        if( rit == objectMap.rend() )
        {
            bRescan = true;           
            rit = objectMap.rbegin();
        }

        ObjectKnowledge* pObject = (*rit).second;
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
// Name: DefaultMapEventHandler::IsObjectKnowledgeAtPos
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
bool DefaultMapEventHandler::IsObjectKnowledgeAtPos( const ObjectKnowledge& object, const MT_Vector2D& vGLPos, float rDistancePerPixel )
{
    Object_ABC* pObject = object.GetRealObject();
    if( pObject != 0 )
        return IsObjectAtPos( *pObject, vGLPos, rDistancePerPixel );
    else
        return Tools::PointNearLine( vGLPos, object.GetPointList(), 9.0 * rDistancePerPixel );
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::PopupMenu
/** @param  vPos 
*/
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
void DefaultMapEventHandler::PopupMenu( const MT_Vector2D& vGLPos, float rDistancePerPixel )
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
        disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
        emit ElementSelected( selectedElement_ );
        connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    }

    // Delete the old menu (necessary for correctly deleting any submenus).
    delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu();

    MT_Vector2D vGLPos2 = vGLPos;
    // Give other interface elements a chance to fill the popup menu.
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement_, &vGLPos2 ) );

    // Add all the lima / limit creation commands to the popup menu.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->insertSeparator();

    int nId = pPopupMenu_->insertItem( tr( "Créer limite" ), this, SLOT( SwitchToLineCreation( int ) ) );
    pPopupMenu_->setItemParameter( nId, 999 );

    QPopupMenu* pCreateLimaMenu = new QPopupMenu( pPopupMenu_ );
    for( int n = 0; n < eLimaFuncNbr; ++n )
    {
        int nId = pCreateLimaMenu->insertItem( Tools::ToString( (E_FuncLimaType)n ), this, SLOT( SwitchToLineCreation( int ) ) ); 
        pCreateLimaMenu->setItemParameter( nId, n );
    }
    pPopupMenu_->insertItem( tr( "Créer lima" ), pCreateLimaMenu );

    // Finally popup the menu.
    pPopupMenu_->popup( QCursor::pos() );
}
