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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationMapFilter.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:49 $
// $Revision: 10 $
// $Workfile: MOS_AgentCreationMapFilter.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AgentCreationMapFilter.h"
#include "moc_MOS_AgentCreationMapFilter.cpp"

#include "MOS_AgentCreationPanel.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_MainWindow.h"
#include "MOS_MapDropEvent.h"
#include "MOS_MapMouseEvent.h"
#include "MOS_World.h"
#include "MOS_Nature.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter constructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationMapFilter::MOS_AgentCreationMapFilter( MOS_AgentCreationPanel& creationPanel )
    : QObject               ( &creationPanel )
    , MOS_MapEventFilter_ABC( false )
    , creationPanel_        ( creationPanel )
{
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this, SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter destructor
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
MOS_AgentCreationMapFilter::~MOS_AgentCreationMapFilter()
{
//    Pop();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
void MOS_AgentCreationMapFilter::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::eventFilter
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool MOS_AgentCreationMapFilter::eventFilter( QObject* /*pSender*/, QEvent* pEvent )
{
    switch( pEvent->type() )
    {
        case QEvent::DragEnter:
            return OnDragEnter( *(QDragEnterEvent*)pEvent );

        case QEvent::Drop:
            return OnDrop( *(MOS_MapDropEvent*)pEvent );

        case QEvent::MouseButtonPress:
            return OnMousePress( *(MOS_MapMouseEvent*)pEvent );

        case QEvent::MouseMove:
            return OnMouseMove( *(MOS_MapMouseEvent*)pEvent );
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::OnDragEnter
/** @param  dragEnterEvent 
    @return 
    */
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool MOS_AgentCreationMapFilter::OnDragEnter( QDragEnterEvent& dragEnterEvent )
{
    dragEnterEvent.accept( dragEnterEvent.provides( "pointer/agentType" ) );
    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::OnDrop
/** @param  dropEvent 
    @return 
    */
// Created: APE 2004-09-02
// -----------------------------------------------------------------------------
bool MOS_AgentCreationMapFilter::OnDrop( const MOS_MapDropEvent& mapDropEvent )
{
    assert( mapDropEvent.provides( "pointer/agentType" ) );

    MOS_AgentCreationPanel::MOS_AgentType* pAgentType;
    QByteArray tmp = mapDropEvent.encodedData( "pointer/agentType" );
    memcpy( &pAgentType, tmp.data(), sizeof( pAgentType ) );

    MOS_SelectedElement& selectedElement = creationPanel_.GetSelectedElement();
    assert( selectedElement.pGtia_ != 0 || selectedElement.pAgent_ != 0 );
    MOS_Gtia* pGtia = (selectedElement.pGtia_ != 0) ? selectedElement.pGtia_ : & selectedElement.pAgent_->GetGtia();
    MOS_Agent& newAgent = CreateAgent( *pAgentType, mapDropEvent.GetMapPos(), *pGtia, selectedElement.pAgent_ );

    // If pressed with ctrl & the agent is an automate, add its children units.
    BYTE stateBuffer[256];
    GetKeyboardState(stateBuffer);
    if( stateBuffer[VK_CONTROL] & 0x80 )
    {
        float rDispersion = creationPanel_.GetDispersionRadius();
        MT_Vector2D vPos = mapDropEvent.GetMapPos();
        for( MOS_AgentCreationPanel::IT_AgentTypeVector it = pAgentType->composition_.begin(); it != pAgentType->composition_.end(); ++it )
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
// Name: MOS_AgentCreationMapFilter::OnMousePress
/** @param  mapMouseEvent 
    @return 
    */
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
bool MOS_AgentCreationMapFilter::OnMousePress( const MOS_MapMouseEvent& mapMouseEvent )
{
    vOldMousePos_ = mapMouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::OnMouseMove
/** @param  mapMouseEvent 
    @return 
    */
// Created: APE 2004-09-22
// -----------------------------------------------------------------------------
bool MOS_AgentCreationMapFilter::OnMouseMove( const MOS_MapMouseEvent& mapMouseEvent )
{
    if( selectedElement_.pAgent_ != 0 && mapMouseEvent.state() == (LeftButton | ShiftButton) )
    {
        selectedElement_.pAgent_->SetPos( selectedElement_.pAgent_->GetPos() + mapMouseEvent.GetMapPos() - vOldMousePos_ );
        vOldMousePos_ = mapMouseEvent.GetMapPos();
        return true;
    }
    vOldMousePos_ = mapMouseEvent.GetMapPos();
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCreationMapFilter::CreateAgent
/** @param  agentType 
    @param  vPos 
    @param  gtia 
    @param  pParent 
*/
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
MOS_Agent& MOS_AgentCreationMapFilter::CreateAgent( const MOS_AgentCreationPanel::MOS_AgentType& agentType, const MT_Vector2D& vPos, MOS_Gtia& gtia, MOS_Agent* pParent )
{
    MOS_Agent* pNewAgent = new MOS_Agent( true );
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
	pNewAgent->pTypeAutomate_ = MOS_App::GetApp().GetAgentManager().FindTypeAutomate( agentType.strName_ );
    if ( pNewAgent->IsAutomate() )
        pNewAgent->pTypePion_ = & pNewAgent->pTypeAutomate_->GetTypePC();
    else
        pNewAgent->pTypePion_ = MOS_App::GetApp().GetAgentManager().FindTypePion( agentType.strName_ );
    pNewAgent->InitializeStocks();

//$$$$$$
//    MOS_AgentModel* pModel = MOS_App::GetApp().GetAgentManager().FindModel( agentType.strDIAModel_ );
//    assert( pModel != 0 ); //$$$ throw
//    pNewAgent->SetModel( *pModel );

    MOS_App::GetApp().GetAgentManager().AddAgent( *pNewAgent );
    MOS_App::GetApp().NotifyAgentCreated( *pNewAgent );

    return *pNewAgent;
}

