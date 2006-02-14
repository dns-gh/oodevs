// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeFactory_h_
#define __ObjectKnowledgeFactory_h_

#include "ASN_Types.h"
#include "Resolver_ABC.h"

class ObjectKnowledge;
class Agent;
class Object_ABC;
class Controller;

// =============================================================================
/** @class  ObjectKnowledgeFactory
    @brief  ObjectKnowledgeFactory
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledgeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeFactory( Controller& controller
                                   , const Resolver_ABC< Agent >& agentResolver
                                   , const Resolver_ABC< Object_ABC >& objectResolver );
    virtual ~ObjectKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual ObjectKnowledge* Create( const ASN1T_MsgObjectKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeFactory( const ObjectKnowledgeFactory& );            //!< Copy constructor
    ObjectKnowledgeFactory& operator=( const ObjectKnowledgeFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& agentResolver_;
    const Resolver_ABC< Object_ABC >& objectResolver_;
    //@}
};

#endif // __ObjectKnowledgeFactory_h_
