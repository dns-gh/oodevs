// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledges_h_
#define __AgentKnowledges_h_

#include "ASN_Types.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"
#include "Resolver.h"

class AgentKnowledge;
class AgentKnowledgeFactory_ABC;
class Controller;
class KnowledgeGroup;

// =============================================================================
/** @class  AgentKnowledges
    @brief  AgentKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledges : public Extension_ABC
                      , public Updatable_ABC< ASN1T_MsgUnitKnowledgeCreation >
                      , public Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
                      , public Updatable_ABC< ASN1T_MsgUnitKnowledgeDestruction >
                      , public Resolver< AgentKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledges( Controller& controller, const KnowledgeGroup& group, AgentKnowledgeFactory_ABC& factory );
    virtual ~AgentKnowledges();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledges( const AgentKnowledges& );            //!< Copy constructor
    AgentKnowledges& operator=( const AgentKnowledges& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeCreation&    message );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate&      message );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const KnowledgeGroup& group_;
    AgentKnowledgeFactory_ABC& factory_;
    //@}
};

#endif // __AgentKnowledges_h_
