// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DISPATCHER_LOGISTIC_HIERARCHY_H__
#define __DISPATCHER_LOGISTIC_HIERARCHY_H__

#include "LogisticLink.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver_ABC.h"
#include "tools/Resolver.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace sword
{
    class ChangeLogisticLinks;
    class LogSupplyQuotas;
    class ParentEntity;
    class SeqOfDotationQuota;
}

namespace dispatcher
{
    class Automat_ABC;
    class Formation_ABC;
    class DotationQuota;
    class LogisticHierarchyOwner_ABC;
    class LogisticEntity;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LogisticHierarchy
    @brief  LogisticHierarchy
*/
// Created: AHC 2010-10
// =============================================================================
class LogisticHierarchy : private boost::noncopyable
                        , public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< sword::ChangeLogisticLinks >
                        , public kernel::Updatable_ABC< sword::LogSupplyQuotas >
{
public:
    //! @name Constructor/destructor
    //@{
             LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< Automat_ABC >& automates );
    virtual ~LogisticHierarchy();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ChangeLogisticLinks& msg );
    virtual void DoUpdate( const sword::LogSupplyQuotas& msg );

    void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Tools
    //@{
    LogisticEntity* FindLogisticEntity( const sword::ParentEntity& msg ) const;
    LogisticLink*   FindLogisticLink  ( const LogisticEntity& superior ) const;
    void            UpdateQuotas      ( tools::Resolver< DotationQuota >& quotas, const sword::SeqOfDotationQuota& msg );
    void            SendQuotas        ( ClientPublisher_ABC& publisher, const tools::Resolver< DotationQuota >& quotas, const LogisticEntity& supplier ) const;
    //@}

private:
    //! @name Member data
    //@{
    const LogisticHierarchyOwner_ABC& owner_;
    const tools::Resolver_ABC< Formation_ABC >& formations_;
    const tools::Resolver_ABC< Automat_ABC >& automats_;

    std::vector< boost::shared_ptr< LogisticLink > > superiorLinks_;
    //@}
};

} // namespace dispatcher

#endif // __DISPATCHER_LOGISTIC_HIERARCHY_H__
