 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledge_h_
#define __UrbanKnowledge_h_

#include "UrbanKnowledge_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Entity_ABC;
    class ModelVisitor_ABC;
}

namespace sword
{
    class UrbanKnowledgeCreation;
    class UrbanKnowledgeUpdate;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;
    class Object_ABC;

// =============================================================================
/** @class  UrbanKnowledge
    @brief  UrbanKnowledge
*/
// Created: MGD 2009-12-11
// =============================================================================
class UrbanKnowledge : public UrbanKnowledge_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UrbanKnowledgeCreation >
                     , public kernel::Updatable_ABC< sword::UrbanKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge( const Model_ABC& model, const sword::UrbanKnowledgeCreation& message );
    virtual ~UrbanKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanKnowledgeCreation& message );
    virtual void DoUpdate( const sword::UrbanKnowledgeUpdate& message );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const Entity_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC& GetOwner() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    //@}

    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned perceivedPresent : 1;
        unsigned automat_perceptionPresent : 1;
        unsigned progressPresent : 1;
        unsigned maxProgressPresent : 1;
    };

private:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    const kernel::Team_ABC& team_;
    const Object_ABC* pUrban_;
    unsigned int rProgress_;
    unsigned int rMaxProgress_;
    bool bPerceived_;
    std::vector< const kernel::Automat_ABC* > automatPerceptions_;
    T_Optionals optionals_;
    //@}
};

}

#endif // __UrbanKnowledge_h_
