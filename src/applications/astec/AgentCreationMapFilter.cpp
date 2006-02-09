// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationMapFilter.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:49 $
// $Revision: 10 $
// $Workfile: AgentCreationMapFilter.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentCreationMapFilter.h"
#include "moc_AgentCreationMapFilter.cpp"

#include "AgentCreationPanel.h"
#include "Agent.h"
#include "Gtia.h"
#include "App.h"
#include "AgentManager.h"
#include "MainWindow.h"
#include "MapDropEvent.h"
#include "MapMouseEvent.h"
#include "World.h"
#include "Nature.h"

// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter constructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationMapFilter::AgentCreationMapFilter( AgentCreationPanel& creationPanel )
    : QObject               ( &creationPanel )
    , MapEventFilter_ABC( false )
    , creationPanel_        ( creationPanel )
{
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this, SLOT( SetSelectedElement( SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
AgentCreationMapFilter::~AgentCreationMapFilter()
{
//    Pop();
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
void AgentCreationMapFilter::SetSelectedElement( SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::eventFilter
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool AgentCreationMapFilter::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::DragEnter:
            return OnDragEnter( *(QDragEnterEvent*)pEvent );

        case QEvent::Drop:
            return OnDrop( *(MapDropEvent*)pEvent );

        case QEvent::MouseButtonPress:
            return OnMousePress( *(MapMouseEvent*)pEvent );

        case QEvent::MouseMove:
            return OnMouseMove( *(MapMouseEvent*)pEvent );
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::OnDragEnter
/** @param  dragEnterEvent 
    @return 
    */
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool AgentCreationMapFilter::OnDragEnter( QDragEnterEvent& dragEnterEvent )
{
    dragEnterEvent.accept( dragEnterEvent.provides( "pointer/agentType" ) );
    return true;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::OnDrop
/** @param  dropEvent 
    @return 
    */
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool AgentCreationMapFilter::OnDrop( const MapDropEvent& mapDropEvent )
{
    assert( mapDropEvent.provides( "pointer/agentType" ) );

    AgentCreationPanel::AgentType* pAgentType;
    QByteArray tmp = mapDropEvent.encodedData( "pointer/agentType" );
    memcpy( &pAgentType, tmp.data(), sizeof( pAgentType ) );

    SelectedElement& selectedElement = creationPanel_.GetSelectedElement();
    assert( selectedElement.pGtia_ != 0 || selectedElement.pAgent_ != 0 );
    Gtia* pGtia = (selectedElement.pGtia_ != 0) ? selectedElement.pGtia_ : & selectedElement.pAgent_->GetGtia();
    Agent& newAgent = CreateAgent( *pAgentType, mapDropEvent.GetMapPos(), *pGtia, selectedElement.pAgent_ );

    // If pressed with ctrl & the agent is an automate, add its children units.
    BYTE stateBuffer[256];
    GetKeyboardState(stateBuffer);
    if( stateBuffer[VK_CONTROL] & 0x80 )
    {
        float rDispersion = creationPanel_.GetDispersionRadius();
        MT_Vector2D vPos = mapDropEvent.GetMapPos();
        for( AgentCreationPanel::IT_AgentTypeVector it = pAgentType->composition_.begin(); it != pAgentType->composition_.end(); ++it )
        {

            MT_Vector2D vNewPoint;
            do
            {
                vNewPoint.rX_ = vPos.rX_ + ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f ) * rDispersion;
                vNewPoint.rY_ = vPos.rY_ + ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f ) * rDispersion;
            } while( vNewPoint.Distance( vPos ) > rDispersion );
            CreateAgent( **it, vNewPoint, *pGtia, &newAgent );
        }
    }

    return true;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::OnMousePress
/** @param  mapMouseEvent 
    @return 
    */
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
bool AgentCreationMapFilter::OnMousePress( const MapMouseEvent& mapMouseEvent )
{
    vOldMousePos_ = mapMouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::OnMouseMove
/** @param  mapMouseEvent 
    @return 
    */
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
bool AgentCreationMapFilter::OnMouseMove( const MapMouseEvent& mapMouseEvent )
{
    if( selectedElement_.pAgent_ != 0 && mapMouseEvent.state() == (LeftButton | ShiftButton) )
    {
        if( selectedElement_.pAgent_->IsAutomate() && selectedElement_.pAgent_->IsEmbraye() && selectedElement_.pAgent_->IsAggregated() )
        {
            Agent::T_AgentVector children = selectedElement_.pAgent_->GetChildren();
            for( Agent::CIT_AgentVector it = children.begin(); it != children.end(); ++it )
                (*it)->SetPos( (*it)->GetPos() + mapMouseEvent.GetMapPos() - vOldMousePos_ );
        }
        selectedElement_.pAgent_->SetPos( selectedElement_.pAgent_->GetPos() + mapMouseEvent.GetMapPos() - vOldMousePos_ );
        vOldMousePos_ = mapMouseEvent.GetMapPos();
        return true;
    }
    vOldMousePos_ = mapMouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: AgentCreationMapFilter::CreateAgent
/** @param  agentType 
    @param  vPos 
    @param  gtia 
    @param  pParent 
*/
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
Agent& AgentCreationMapFilter::CreateAgent( const AgentCreationPanel::AgentType& agentType, const MT_Vector2D& vPos, Gtia& gtia, Agent* pParent )
{
    Agent* pNewAgent = new Agent( true );
    pNewAgent->SetName( agentType.strName_ );
    pNewAgent->SetCategory( agentType.strName_ );
    pNewAgent->SetPos( vPos );
    pNewAgent->SetGtia( gtia );
    pNewAgent->SetParent( pParent );
    pNewAgent->SetIsAutomate( pParent == 0 );
    pNewAgent->SetEmbraye( pParent == 0 );
    if( pParent != 0 )
        pParent->AddChild( *pNewAgent );
    pNewAgent->symbolName_      = agentType.pNature_->symbol_;
    pNewAgent->levelSymbolName_ = agentType.pNature_->levelSymbol_;
	pNewAgent->pTypeAutomate_ = App::GetApp().GetAgentManager().FindTypeAutomate( agentType.strName_ );
    if ( pNewAgent->IsAutomate() )
        pNewAgent->pTypePion_ = & pNewAgent->pTypeAutomate_->GetTypePC();
    else
        pNewAgent->pTypePion_ = App::GetApp().GetAgentManager().FindTypePion( agentType.strName_ );
    pNewAgent->InitializeStocks();

//$$$$$$
//    AgentModel* pModel = App::GetApp().GetAgentManager().FindModel( agentType.strDIAModel_ );
//    assert( pModel != 0 ); //$$$ throw
//    pNewAgent->SetModel( *pModel );

    App::GetApp().GetAgentManager().AddAgent( *pNewAgent );
    App::GetApp().NotifyAgentCreated( *pNewAgent );

    return *pNewAgent;
}

