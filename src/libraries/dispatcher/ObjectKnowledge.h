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

#include "Localisation.h"
#include "ObjectKnowledge_ABC.h"
#include "ObjectAttributeContainer.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace sword
{
    class ObjectKnowledgeCreation;
    class ObjectKnowledgeUpdate;
}

namespace kernel
{
    class Automat_ABC;
    class KnowledgeGroup_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObjectKnowledge : public dispatcher::ObjectKnowledge_ABC
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::ObjectKnowledgeCreation >
                      , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const Model_ABC& model, const sword::ObjectKnowledgeCreation& asnMsg );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeCreation& asnMsg );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& asnMsg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::Object_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC& GetOwner() const;
    virtual QString GetTypeName() const;
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
        unsigned char realObjectPresent : 1;
        unsigned char relevancePresent : 1;
        unsigned char locationPresent : 1;
        unsigned char perceivedPresent : 1;
        unsigned char specific_attributesPresent : 1;
        unsigned char automat_perceptionPresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    const kernel::Team_ABC& owner_;
    const kernel::Object_ABC* pObject_;
    unsigned long entityId_;
    const std::string nType_;
    const kernel::KnowledgeGroup_ABC* knowledgeGroup_;
    const QString typename_;
    unsigned int nRelevance_;
    Localisation localisation_;
    bool bPerceived_;
    std::vector< const kernel::Automat_ABC* > automatPerceptions_;
    ObjectAttributeContainer attributes_;
    T_Optionals optionals_;
    //@}
};

}

#endif // __ObjectKnowledge_h_
