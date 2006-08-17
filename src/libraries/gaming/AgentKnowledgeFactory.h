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
#include "PopulationKnowledgeFactory_ABC.h"

class Controllers;
class Model;
class CoordinateConverter_ABC;

// =============================================================================
/** @class  AgentKnowledgeFactory
    @brief  AgentKnowledgeFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledgeFactory : public AgentKnowledgeFactory_ABC
                            , public PopulationKnowledgeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeFactory( Controllers& controllers, Model& model, const CoordinateConverter_ABC& converter );
    virtual ~AgentKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentKnowledge*      CreateAgentKnowledge     ( const KnowledgeGroup_ABC& group, const ASN1T_MsgUnitKnowledgeCreation& message );
    virtual PopulationKnowledge* CreatePopulationKnowledge( const KnowledgeGroup_ABC& group, const ASN1T_MsgPopulationKnowledgeCreation& message );
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
    Controllers& controllers_;
    Model& model_;
    const CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __AgentKnowledgeFactory_h_
