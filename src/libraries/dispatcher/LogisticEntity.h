// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DISPATCHER_LOGISTIC_ENTITY_H__
#define __DISPATCHER_LOGISTIC_ENTITY_H__

#include "LogisticHierarchyOwner_ABC.h"
#include "LogisticHierarchy.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class LogisticLevel;
}

namespace sword
{
    class ParentEntity;
    class AutomatCreation;
    class FormationCreation;
}

namespace dispatcher
{
    class Automat_ABC;
    class Formation_ABC;
    class LogisticEntityOwner_ABC;

// =============================================================================
/** @class  LogisticEntity
    @brief  LogisticEntity
*/
// Created: AHC 2010-10
// =============================================================================
class LogisticEntity : public kernel::Extension_ABC
                     , public LogisticHierarchyOwner_ABC
{
public:
    //! @name Constructor/destructor
    //@{
             LogisticEntity( const LogisticEntityOwner_ABC& owner, const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< Automat_ABC >& automates, const kernel::LogisticLevel& logisticLevel );
    virtual ~LogisticEntity();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
                  LogisticHierarchy&     GetLogisticHierarchy();
    //@}

    //! @name Network
    //@{
    virtual void Send( sword::ParentEntity& msg ) const;
    void Send( sword::AutomatCreation& msg ) const;
    void Send( sword::FormationCreation& msg ) const;

    void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Member data
    //@{  
    const LogisticEntityOwner_ABC& owner_;
    const kernel::LogisticLevel& logisticLevel_;
    LogisticHierarchy logisticHierarchy_;
    //@}
};

} // namespace dispatcher

#endif // __DISPATCHER_LOGISTIC_ENTITY_H__

