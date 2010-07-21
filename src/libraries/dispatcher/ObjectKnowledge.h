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
class ObjectKnowledge : public dispatcher::ObjectKnowledge_ABC
                      , public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeCreation >
                      , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg );
    virtual ~ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& asnMsg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Object_ABC* GetEntity() const;
    virtual const kernel::Team_ABC& GetOwner() const;
    virtual QString GetTypeName() const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void CreateOrUpdate( const Common::MsgObjectAttributes& message );
    template< typename T >
    void CreateOrUpdate( const Common::MsgObjectAttributes& message, const Model_ABC& model );
    void Initialize( const Model_ABC& model, const Common::MsgObjectAttributes& message );
    void AddAttribute( ObjectAttribute_ABC* attribute );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    //@}

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

    //! @name Types
    //@{
    typedef boost::ptr_vector< ObjectAttribute_ABC > T_ObjectAttributes;
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    const kernel::Team_ABC& team_;
    const kernel::Object_ABC* pObject_;
    const std::string nType_;
    const kernel::KnowledgeGroup_ABC* knowledgeGroup_;
    const QString typename_;
    unsigned int nRelevance_;
    Localisation localisation_;
    bool bPerceived_;
    std::vector< const kernel::Automat_ABC* > automatPerceptions_;
    T_ObjectAttributes attributes_;
    T_Optionals optionals_;
    //@}
};

}

#endif // __ObjectKnowledge_h_
