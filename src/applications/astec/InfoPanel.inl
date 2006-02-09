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
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel.inl $
// $Author: Age $
// $Modtime: 5/04/05 15:25 $
// $Revision: 5 $
// $Workfile: InfoPanel.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: InfoPanel::GetAgentKnowledgePanel
/** @return 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
inline
AgentKnowledgePanel& InfoPanel::GetAgentKnowledgePanel()
{
    assert( pAgentKnowledgePanel_ != 0 );
    return *pAgentKnowledgePanel_;
}


// -----------------------------------------------------------------------------
// Name: InfoPanel::GetObjectKnowledgePanel
/** @return 
*/
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
inline
ObjectKnowledgePanel& InfoPanel::GetObjectKnowledgePanel()
{
    assert( pObjectKnowledgePanel_ != 0 );
    return *pObjectKnowledgePanel_;
}

// -----------------------------------------------------------------------------
// Name: InfoPanel::sizeHint
/** @return 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
inline
QSize InfoPanel::sizeHint() const
{
    return QSize( 230, 340 );
}

