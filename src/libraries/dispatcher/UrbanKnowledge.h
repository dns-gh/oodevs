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

#include "SimpleEntity.h"
#include "UrbanObject.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"
#include "protocol/Simulation.h"

namespace kernel
{
    class Automat_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  UrbanKnowledge
    @brief  UrbanKnowledge
*/
// Created: MGD 2009-12-11
// =============================================================================
class UrbanKnowledge : public SimpleEntity< kernel::UrbanKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgUrbanKnowledgeCreation& message );
    virtual ~UrbanKnowledge();
    //@}

    //! @name Operations
    //@{
    using SimpleEntity< kernel::UrbanKnowledge_ABC >::Update;
    void Update( const MsgsSimToClient::MsgUrbanKnowledgeCreation& message );
    void Update( const MsgsSimToClient::MsgUrbanKnowledgeUpdate& message );
    void SendCreation( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const UrbanObject* GetEntity() const;
    virtual const kernel::Team_ABC& GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Types
    //@{
    //$$$ bullshit
    struct T_Optionals
    {
        unsigned relevancePresent : 1;
        unsigned perceivedPresent : 1;
        unsigned automat_perceptionPresent : 1;
        unsigned identification_levelPresent : 1;
        unsigned progressPresent : 1;
    };
    //@}

public:
    //! @name Member data
    //@{
    const Model_ABC& model_;
    const kernel::Team_ABC& team_;
    const UrbanObject* pUrban_;
    unsigned int nRelevance_;
    unsigned int nProgress_;
    bool bPerceived_;
    MsgsSimToClient::EnumUnitIdentificationLevel nIdentificationLevel_;
    std::vector< const kernel::Automat_ABC* > automatPerceptions_;
    T_Optionals optionals_;
    //@}

};

}

#endif // __UrbanKnowledge_h_
