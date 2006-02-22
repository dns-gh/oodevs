// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentKnowledgePanel.h $
// $Author: Age $
// $Modtime: 5/04/05 18:35 $
// $Revision: 8 $
// $Workfile: AgentKnowledgePanel.h $
//
// *****************************************************************************

#ifndef __AgentKnowledgePanel_h_
#define __AgentKnowledgePanel_h_

#include "Types.h"
#include "InfoPanel_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "ListView.h"
#include "SelectionObserver_ABC.h"

class Controller;
class ActionController;
class AgentKnowledges;
class AgentKnowledge;
class Displayer;
class KnowledgeGroup;
class Agent;
class PerceptionMap;
class Perception;

// =============================================================================
/** @class  AgentKnowledgePanel
    @brief  AgentKnowledgePanel
*/
// Created: APE 2004-05-03
// =============================================================================
class AgentKnowledgePanel : public InfoPanel_ABC
                          , private Observer_ABC
                          , public ElementObserver_ABC< AgentKnowledges >
                          , public ElementObserver_ABC< AgentKnowledge >
                          , public ElementObserver_ABC< PerceptionMap >
                          , public SelectionObserver_ABC
                          , public SelectionObserver_Base< KnowledgeGroup >
                          , public SelectionObserver_Base< Agent >
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgePanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~AgentKnowledgePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const AgentKnowledge& k, ValuedListItem* item );
    void Display( const Perception& perception, ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentKnowledgePanel( const AgentKnowledgePanel& );
    AgentKnowledgePanel& operator=( const AgentKnowledgePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AgentKnowledges& knowledges );
    virtual void NotifyUpdated( const AgentKnowledge& knowledge );
    virtual void NotifyUpdated( const PerceptionMap& perceptions );
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const KnowledgeGroup& element );
    virtual void Select( const Agent& element );
    void Select( const AgentKnowledges* k );
    void Display( const AgentKnowledge& k );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( QListViewItem* );
    //@}

private:
    //! @name Member data
    //@{
    ListView< AgentKnowledgePanel >* pKnowledgeListView_;
    Displayer* display_;
    ListView< AgentKnowledgePanel >* pPerceptionListView_;

    const AgentKnowledges* selected_;
    const AgentKnowledges* newSelection_;
    const AgentKnowledge* subSelected_;
//    Gtia* pGtia_;
//    QCheckBox* pOwnTeamCheckBox_;
//    AgentKnowledge* pSelectedKnowledge_;
    
//    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __AgentKnowledgePanel_h_
