// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPopulationKnowledge_h_
#define __ParamPopulationKnowledge_h_

#include "EntityParameter.h"

class PopulationKnowledge;
class Population;
class AgentKnowledgeConverter_ABC;
class Agent_ABC;
class Entity_ABC;

// =============================================================================
/** @class  ParamPopulationKnowledge
    @brief  ParamPopulationKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamPopulationKnowledge : public EntityParameter< PopulationKnowledge >
                               , public ContextMenuObserver_ABC< Population >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPopulationKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Entity_ABC& agent );
    virtual ~ParamPopulationKnowledge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamPopulationKnowledge( const ParamPopulationKnowledge& );            //!< Copy constructor
    ParamPopulationKnowledge& operator=( const ParamPopulationKnowledge& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Population& entity, QPopupMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const Agent_ABC& agent_;
    //@}
};

#endif // __ParamPopulationKnowledge_h_
