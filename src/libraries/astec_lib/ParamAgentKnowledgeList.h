// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentKnowledgeList_h_
#define __ParamAgentKnowledgeList_h_

#include "ASN_Types.h"
#include "EntityListParameter.h"

class AgentKnowledge;
class Agent;
class Agent_ABC;
class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamAgentKnowledgeList
    @brief  ParamAgentKnowledgeList
    // $$$$ AGE 2006-03-14: factor these stuffs with single entity parameters 
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledgeList : public EntityListParameter< AgentKnowledge >
                              , public ContextMenuObserver_ABC< Agent > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeAgent& asn, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Agent_ABC& agent );
    virtual ~ParamAgentKnowledgeList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentKnowledgeList( const ParamAgentKnowledgeList& );            //!< Copy constructor
    ParamAgentKnowledgeList& operator=( const ParamAgentKnowledgeList& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Agent& entity, QPopupMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const Agent& agent_;
    //@}
};

#endif // __ParamAgentKnowledgeList_h_
