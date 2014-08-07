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

#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include <tools/Resolver.h>

namespace sword
{
    class ObjectKnowledgeCreation;
    class ObjectKnowledgeUpdate;
}

namespace kernel
{
    class Displayer_ABC;
    class Controller;
    class Agent_ABC;
    class ObjectType;
    class Team_ABC;
}

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledge : public gui::EntityImplementation< kernel::ObjectKnowledge_ABC >
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
                      , public kernel::Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const kernel::Entity_ABC& owner, const sword::ObjectKnowledgeCreation& message,
                              kernel::Controller& controller,
                              const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver,
                              const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver,
                              const kernel::ObjectType& type );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual const kernel::Object_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::Entity_ABC& GetOwner() const;
    virtual std::string GetSymbol() const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledge( const ObjectKnowledge& );            //!< Copy constructor
    ObjectKnowledge& operator=( const ObjectKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver_;
    const kernel::Entity_ABC& owner_;
    const kernel::ObjectType* type_;
    std::string position_;
    unsigned long entityId_;
    const kernel::Team_ABC* pTeam_;
    std::string symbol_;
    kernel::OptionalValue< bool > bIsPerceived_;
    kernel::OptionalValue< uint > nRelevance_;
    QString objectName_;
    //@}
};

#endif // __ObjectKnowledge_h_
