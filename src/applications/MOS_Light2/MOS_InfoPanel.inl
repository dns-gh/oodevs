// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_InfoPanel.inl $
// $Author: Age $
// $Modtime: 5/04/05 15:25 $
// $Revision: 5 $
// $Workfile: MOS_InfoPanel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::GetAgentKnowledgePanel
/** @return 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
inline
MOS_AgentKnowledgePanel& MOS_InfoPanel::GetAgentKnowledgePanel()
{
    assert( pAgentKnowledgePanel_ != 0 );
    return *pAgentKnowledgePanel_;
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::GetObjectKnowledgePanel
/** @return 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
inline
MOS_ObjectKnowledgePanel& MOS_InfoPanel::GetObjectKnowledgePanel()
{
    assert( pObjectKnowledgePanel_ != 0 );
    return *pObjectKnowledgePanel_;
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::sizeHint
/** @return 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
inline
QSize MOS_InfoPanel::sizeHint() const
{
    return QSize( 230, 340 );
}

