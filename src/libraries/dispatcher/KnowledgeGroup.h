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
#include <tools/Resolver.h>

namespace sword
{
    class KnowledgeGroupCreation;
    class KnowledgeGroupUpdate;
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
                     , public kernel::Updatable_ABC< sword::KnowledgeGroupUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( Model_ABC& model, const sword::KnowledgeGroupCreation& msg );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept         ( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void DoUpdate( const sword::KnowledgeGroupUpdate& message ); // LTO
    virtual bool IsActivated() const;
    virtual bool IsCrowd() const;
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
    bool crowd_;
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< dispatcher::Automat_ABC > automats_;
    //@}
};

}

#endif // __KnowledgeGroup_h_
