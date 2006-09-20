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

namespace kernel
{
    class Displayer_ABC;
    class Object_ABC;
    class Controller;
    class Agent_ABC;
    class Team_ABC;
    class CoordinateConverter_ABC;
    class ObjectType;
    class InstanciationComplete;
}

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public kernel::Entity_ABC
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                      , public kernel::Updatable_ABC< kernel::InstanciationComplete >
                      , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message,
                              kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                              const kernel::Resolver_ABC< kernel::Object_ABC >& objectResolver,
                              const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, 
                              const kernel::Resolver_ABC< kernel::ObjectType >& typeResolver );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;

    void Display( kernel::Displayer_ABC& displayer ) const;
    void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    bool IsInTeam( const kernel::Team_ABC& team ) const;
    const kernel::Team_ABC& GetTeam() const; // $$$$ AGE 2006-09-15: 

    bool KnowledgeIsInTeam( const kernel::Team_ABC& team ) const;
    const kernel::Team_ABC* GetKnowledgeTeam() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    const kernel::Object_ABC* GetRealObject() const; // $$$$ AGE 2006-09-15: 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignement operator
    //@}
    
    //! @name Types
    //@{
    typedef std::set< const kernel::Agent_ABC* > T_Agents;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver_;
    kernel::Controller& controller_;
    const kernel::Team_ABC& owner_;

    unsigned long id_;
    const kernel::ObjectType* type_;

    std::string position_;
    kernel::Object_ABC* pRealObject_;

    kernel::OptionalValue< unsigned int >  nPourcentageConstruction_;
    kernel::OptionalValue< unsigned int >  nPourcentageValorisation_;
    kernel::OptionalValue< unsigned int >  nPourcentageContournement_;
    kernel::OptionalValue< bool >          bEnPreparation_;
    kernel::OptionalValue< bool >          bIsPerceived_;
    kernel::OptionalValue< uint >          nRelevance_;

public: // $$$$ AGE 2006-03-13: 
    T_Agents detectingAutomats_; // $$$$ AGE 2006-03-13: move in an extension ?
    //@}
};

#endif // __ObjectKnowledge_h_
