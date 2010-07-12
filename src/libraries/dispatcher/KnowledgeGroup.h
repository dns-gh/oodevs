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

#include "KnowledgeGroup_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupUpdate;
}

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
*/
// Created: NLD 2006-09-19
// =============================================================================
class KnowledgeGroup : public dispatcher::KnowledgeGroup_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgKnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( Model_ABC& model, const MsgsSimToClient::MsgKnowledgeGroupCreation& msg );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept         ( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void DoUpdate( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message ); // LTO
    virtual bool IsActivated() const { return true; };  // $$$$ _RC_ SLG 2009-12-21: TEMP  // LTO
    virtual void Register( dispatcher::KnowledgeGroup_ABC& knowledgeGroup ); // LTO
    virtual void Remove( dispatcher::KnowledgeGroup_ABC& knowledgeGroup ); // LTO
    virtual void Register( dispatcher::Automat_ABC& automat ); // LTO
    virtual void Remove( dispatcher::Automat_ABC& automat ); // LTO
    //@}

    //! @name Accessors
    //@{
    virtual const dispatcher::Team_ABC& GetTeam() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ChangeSuperior( dispatcher::KnowledgeGroup_ABC* superior );
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    dispatcher::Team_ABC& team_;
    dispatcher::KnowledgeGroup_ABC* parent_;
    std::string type_; // LTO
    bool enabled_; // LTO
    bool jammed_; // LTO
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< dispatcher::Automat_ABC > automats_;
    //@}
};

}

#endif // __KnowledgeGroup_h_
