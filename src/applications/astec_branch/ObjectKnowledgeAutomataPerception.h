// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeAutomataPerception_h_
#define __ObjectKnowledgeAutomataPerception_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Object_ABC;
class Agent;

// =============================================================================
/** @class  ObjectKnowledgeAutomataPerception
    @brief  ObjectKnowledgeAutomataPerception
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledgeAutomataPerception : public Extension_ABC
                                        , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeAutomataPerception( const Resolver_ABC< Agent >& resolver );
    virtual ~ObjectKnowledgeAutomataPerception();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeAutomataPerception( const ObjectKnowledgeAutomataPerception& );            //!< Copy constructor
    ObjectKnowledgeAutomataPerception& operator=( const ObjectKnowledgeAutomataPerception& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::set< const Agent* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const Resolver_ABC< Agent >& resolver_;

    bool set_;
    T_Agents detectingAutomats_;
    //@}
};

#endif // __ObjectKnowledgeAutomataPerception_h_
