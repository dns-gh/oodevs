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

#include "gaming/ASN_Types.h"
#include "EntityListParameter.h"

class AgentKnowledge;
class Agent_ABC;
class Entity_ABC;
class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamAgentKnowledgeList
    @brief  ParamAgentKnowledgeList
    // $$$$ AGE 2006-03-14: factor these stuffs with single entity parameters 
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledgeList : public EntityListParameter< AgentKnowledge >
                              , public ContextMenuObserver_ABC< Agent_ABC > 
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledgeList( QWidget* pParent, ASN1T_ListKnowledgeAgent& asn, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent );
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
    virtual void NotifyContextMenu( const Agent_ABC& entity, ContextMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const Agent_ABC& agent_;
    //@}
};

#endif // __ParamAgentKnowledgeList_h_
