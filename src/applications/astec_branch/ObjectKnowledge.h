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

class Displayer_ABC;
class Object;
class Controller;
class Agent;
class Team;
class CoordinateConverter_ABC;
class ObjectType;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public Entity_ABC
                      , public Extension_ABC
                      , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const Team& owner, const ASN1T_MsgObjectKnowledgeCreation& message,
                              Controller& controller, const CoordinateConverter_ABC& converter,
                              const Resolver_ABC< Object >& objectResolver,
                              const Resolver_ABC< Agent >& agentResolver, 
                              const Resolver_ABC< ObjectType >& typeResolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;

    void Display( Displayer_ABC& displayer ) const;
    void DisplayInList( Displayer_ABC& displayer ) const;
    Object* GetRealObject() const;
    bool IsInTeam( const Team& team ) const;
    bool KnowledgeIsInTeam( const Team& team ) const;
    const Team* GetKnowledgeTeam() const;
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
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter_ABC& converter_;
    const Resolver_ABC< Object >& objectResolver_;
    const Resolver_ABC< Agent >& agentResolver_;
    Controller& controller_;
    const Team& owner_;

    unsigned long id_;
    const ObjectType* type_;

    std::string position_;
    Object* pRealObject_;

    OptionalValue< unsigned int >  nPourcentageConstruction_;
    OptionalValue< unsigned int >  nPourcentageValorisation_;
    OptionalValue< unsigned int >  nPourcentageContournement_;
    OptionalValue< bool >          bEnPreparation_;
    OptionalValue< bool >          bIsPerceived_;
    OptionalValue< uint >          nRelevance_;

public: // $$$$ AGE 2006-03-13: 
    T_Agents detectingAutomats_; // $$$$ AGE 2006-03-13: move in an extension ?
    //@}
};

#endif // __ObjectKnowledge_h_
