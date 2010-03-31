 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_


#include "SimpleEntity.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupUpdate;
    class MsgKnowledgeGroupDestruction;
}

namespace kernel
{
    class ModelVisitor_ABC;
    class Team_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: NLD 2006-09-19
// =============================================================================
class KnowledgeGroup : public SimpleEntity< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( Model_ABC& model, const MsgsSimToClient::MsgKnowledgeGroupCreation& msg );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept         ( kernel::ModelVisitor_ABC& visitor ) const;

    void Update( const MsgsSimToClient::MsgKnowledgeGroupCreation& message );
    void Update( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message ); // LTO
    void Update( const MsgsSimToClient::MsgKnowledgeGroupDestruction& message ); // LTO
    virtual bool IsActivated() const { return true; };  // $$$$ _RC_ SLG 2009-12-21: TEMP  // LTO
    virtual void Register( kernel::KnowledgeGroup_ABC& knowledgeGroup ); // LTO
    virtual void Remove( kernel::KnowledgeGroup_ABC& knowledgeGroup ); // LTO
    virtual void Register( kernel::Automat_ABC& automat ); // LTO
    virtual void Remove( kernel::Automat_ABC& automat ); // LTO
    //@}

    //! @name Accessors
    //@{
    const kernel::Team_ABC& GetTeam() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ChangeSuperior( kernel::KnowledgeGroup_ABC* superior );
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    kernel::Team_ABC& team_;
    kernel::KnowledgeGroup_ABC* parent_;
    std::string type_; // LTO
    bool enabled_; // LTO
    tools::Resolver< kernel::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< kernel::Automat_ABC > automats_;
    //@}
};

}

#endif // __KnowledgeGroup_h_
