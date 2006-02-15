// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#include "ASN_Types.h"
#include "Entity_ABC.h"
#include "Resolver_ABC.h"
#include "OptionalValue.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Object_ABC;
class Controller;
class Agent;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public Entity_ABC
                      , private Extension_ABC
                      , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& message,
                              Controller& controller,
                              const Resolver_ABC< Object_ABC >& objectResolver,
                              const Resolver_ABC< Agent >& agentResolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void UpdateKnowledge( const T& message ) {
        Entity_ABC::Update( message );
    }
    //@}

    //! @name Operations
    //@{
    Object_ABC* GetRealObject() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignement operator
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
    const Resolver_ABC< Object_ABC >& objectResolver_;
    const Resolver_ABC< Agent >& agentResolver_;
    Controller& controller_;

    unsigned long id_;
    unsigned long type_;

    
    T_PointVector points_;
    Object_ABC* pRealObject_;

    OptionalValue< unsigned int >  nPourcentageConstruction_;
    OptionalValue< unsigned int >  nPourcentageValorisation_;
    OptionalValue< unsigned int >  nPourcentageContournement_;
    OptionalValue< bool >          bEnPreparation_;
    OptionalValue< bool >          bIsPerceived_;
    OptionalValue< uint >          nRelevance_;

    T_Agents detectingAutomats_;
    //@}
};

#endif // __ObjectKnowledge_h_
