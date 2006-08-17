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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

class Displayer_ABC;
class Object_ABC;
class Controller;
class Agent_ABC;
class Team_ABC;
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
                      , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message,
                              Controller& controller, const CoordinateConverter_ABC& converter,
                              const Resolver_ABC< Object_ABC >& objectResolver,
                              const Resolver_ABC< Agent_ABC >& agentResolver, 
                              const Resolver_ABC< ObjectType >& typeResolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual std::string GetName() const;

    void Display( Displayer_ABC& displayer ) const;
    void DisplayInList( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;

    bool IsInTeam( const Team_ABC& team ) const;
    bool KnowledgeIsInTeam( const Team_ABC& team ) const;
    const Team_ABC* GetKnowledgeTeam() const;


    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignement operator
    //@}
    
    //! @name Types
    //@{
    typedef std::set< const Agent_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter_ABC& converter_;
    const Resolver_ABC< Object_ABC >& objectResolver_;
    const Resolver_ABC< Agent_ABC >& agentResolver_;
    Controller& controller_;
    const Team_ABC& owner_;

    unsigned long id_;
    const ObjectType* type_;

    std::string position_;
    Object_ABC* pRealObject_;

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
