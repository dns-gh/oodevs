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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DefaultMapEventHandler.inl $
// $Author: Age $
// $Modtime: 17/03/05 14:12 $
// $Revision: 6 $
// $Workfile: MOS_DefaultMapEventHandler.inl $
//
// *****************************************************************************


#include "MOS_Agent.h"
#include "MOS_AgentKnowledge.h"


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::GetObject
/** @return 
*/
// Created: APE 2004-10-05
// -----------------------------------------------------------------------------
inline
QObject* MOS_DefaultMapEventHandler::GetObject()
{
    return this;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::SetSelectedElement
/** @param  selectedElement 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
inline
void MOS_DefaultMapEventHandler::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    selectedElement_ = selectedElement;
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsAgentAtPos
/** @param  vGLPos 
    @return 
*/
// Created: APE 2004-03-12
// -----------------------------------------------------------------------------
inline
bool MOS_DefaultMapEventHandler::IsAgentAtPos( const MOS_Agent& agent, const MT_Vector2D& vGLPos )
{
    //$$$ evil hard coded values! Sure thing they are evil ! Naughty evil values !
    const MT_Vector2D& posAgent = agent.GetPos();
    MT_Rect agentRect( posAgent.rX_ - 300, posAgent.rY_, posAgent.rX_ + 300, posAgent.rY_ + 300 );
    return agentRect.IsInside( vGLPos );
}


// -----------------------------------------------------------------------------
// Name: MOS_DefaultMapEventHandler::IsAgentKnowledgeAtPos
/** @param  agent 
    @param  vGLPos 
    @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
bool MOS_DefaultMapEventHandler::IsAgentKnowledgeAtPos( const MOS_AgentKnowledge& agent, const MT_Vector2D& vGLPos )
{
    //$$$ evil hard coded values!
    const MT_Vector2D& posAgent = agent.GetPosition();
    MT_Rect agentRect( posAgent.rX_ - 300, posAgent.rY_, posAgent.rX_ + 300, posAgent.rY_ + 300 );
    return agentRect.IsInside( vGLPos );
}
