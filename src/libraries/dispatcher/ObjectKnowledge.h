 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_h_
#define __ObjectKnowledge_h_

#include "Localisation.h"
#include "SimpleEntity.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace Common
{
    class MsgObjectAttributes;
}

namespace MsgsSimToClient
{
    class MsgObjectKnowledgeCreation;
    class MsgObjectKnowledgeUpdate;
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
    class ObjectAttribute_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  ObjectKnowledge
    @brief  ObjectKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class ObjectKnowledge : public SimpleEntity< kernel::ObjectKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    using SimpleEntity< kernel::ObjectKnowledge_ABC >::Update;
    void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& asnMsg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Object_ABC* GetEntity() const;
    virtual const kernel::Team_ABC&   GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void CreateOrUpdate( const Common::MsgObjectAttributes& message );
    template< typename T >
    void CreateOrUpdate( const Common::MsgObjectAttributes& message, const Model_ABC& model );
    //@}

    //! @name Attributes
    //@{
    void Initialize( const Model_ABC& model, const Common::MsgObjectAttributes& message );
    void AddAttribute( ObjectAttribute_ABC* attribute );
    //@}

private:
    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned relevancePresent : 1;
        unsigned locationPresent : 1;
        unsigned construction_percentagePresent : 1;
        unsigned mining_percentagePresent : 1;
        unsigned bypass_construction_percentagePresent : 1;
        unsigned reserved_obstacle_activatedPresent : 1;
        unsigned perceivedPresent : 1;
        unsigned specific_attributesPresent : 1;
        unsigned automat_perceptionPresent : 1;
        unsigned construction_dotation_nbrPresent : 1;
        unsigned mining_dotation_nbrPresent : 1;
        unsigned obstacle_typePresent : 1;
    };
    //@}

    const Model_ABC&             model_;
    const kernel::Team_ABC&      team_;
    const kernel::Object_ABC*    pObject_;
    const std::string            nType_;
    const kernel::KnowledgeGroup_ABC* knowledgeGroup_;

    unsigned int                   nRelevance_;
    Localisation                   localisation_;
    bool                           bPerceived_;
    std::vector< const kernel::Automat_ABC* >  automatPerceptions_;

private:
    //! @name Types
    //@{
    typedef boost::ptr_vector< ObjectAttribute_ABC > T_ObjectAttributes;
    //@}

private:
    T_ObjectAttributes           attributes_;
    T_Optionals                  optionals_;
};

}

#endif // __ObjectKnowledge_h_
