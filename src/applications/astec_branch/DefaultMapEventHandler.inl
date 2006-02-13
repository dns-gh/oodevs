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
// $Archive: /MVW_v10/Build/SDK/Light2/src/DefaultMapEventHandler.inl $
// $Author: Age $
// $Modtime: 17/03/05 14:12 $
// $Revision: 6 $
// $Workfile: DefaultMapEventHandler.inl $
//
// *****************************************************************************


#include "Agent.h"
#include "AgentKnowledge.h"


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::GetObject
/** @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
inline
QObject* DefaultMapEventHandler::GetObject()
{
    return this;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
void DefaultMapEventHandler::SetSelectedElement( SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::IsAgentAtPos
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-03-12
// -----------------------------------------------------------------------------
inline
bool DefaultMapEventHandler::IsAgentAtPos( const Agent& agent, const MT_Vector2D& vGLPos )
{
    //$$$ evil hard coded values! Sure thing they are evil ! Naughty evil values !
    if( agent.IsAutomate() && agent.IsAggregated() )
    {
        const MT_Vector2D bary = agent.GetAggregatedPos();
        MT_Rect agentRect( bary.rX_ - 600, bary.rY_, bary.rX_ + 600, bary.rY_ + 600);
        return agentRect.IsInside( vGLPos );
    }
    else
    {
        const MT_Vector2D& posAgent = agent.GetPos();
        MT_Rect agentRect( posAgent.rX_ - 300, posAgent.rY_, posAgent.rX_ + 300, posAgent.rY_ + 300 );
        return agentRect.IsInside( vGLPos );
    }
}


// -----------------------------------------------------------------------------
// Name: DefaultMapEventHandler::IsAgentKnowledgeAtPos
/** @param  agent 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
bool DefaultMapEventHandler::IsAgentKnowledgeAtPos( const AgentKnowledge& agent, const MT_Vector2D& vGLPos )
{
    //$$$ evil hard coded values!
    const MT_Vector2D& posAgent = agent.GetPosition();
    MT_Rect agentRect( posAgent.rX_ - 300, posAgent.rY_, posAgent.rX_ + 300, posAgent.rY_ + 300 );
    return agentRect.IsInside( vGLPos );
}
