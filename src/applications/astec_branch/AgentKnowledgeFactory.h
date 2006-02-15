// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeFactory_h_
#define __AgentKnowledgeFactory_h_

#include "AgentKnowledgeFactory_ABC.h"

class Controller;
class Model;

// =============================================================================
/** @class  AgentKnowledgeFactory
    @brief  AgentKnowledgeFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledgeFactory : public AgentKnowledgeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeFactory( Controller& controller, Model& model );
    virtual ~AgentKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentKnowledge* CreateAgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgeFactory( const AgentKnowledgeFactory& );            //!< Copy constructor
    AgentKnowledgeFactory& operator=( const AgentKnowledgeFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Model& model_;
    //@}
};

#endif // __AgentKnowledgeFactory_h_
