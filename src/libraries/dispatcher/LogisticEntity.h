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

#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "tools/Resolver_ABC.h"
#include "tools/Resolver.h"

namespace Common
{
    class MsgChangeLogisticLinks;
}

namespace client
{
    class ChangeLogisticLinks;
    class LogSupplyQuotas;
}

namespace MsgsSimToClient
{
    class MsgLogSupplyQuotas;
}

namespace kernel
{
    class LogisticLevel;
}

namespace dispatcher
{

    class Automat_ABC;
    class Formation_ABC;
    class DotationQuota;

// =============================================================================
/** @class  LogisticEntity
    @brief  LogisticEntity
*/
// Created: AHC 2010-10
// =============================================================================
class LogisticEntity
    : public kernel::Extension_ABC
    , public kernel::Updatable_ABC< Common::MsgChangeLogisticLinks >
    , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogSupplyQuotas >
{
public:
    //! @name Constructor/destructor
    //@{
    LogisticEntity(const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< Automat_ABC >& automates,
            const kernel::LogisticLevel& level);
    virtual ~LogisticEntity();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const Common::MsgChangeLogisticLinks&         msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogSupplyQuotas&    msg );
    void Fill( client::ChangeLogisticLinks& msg ) const;
    void Fill( client::LogSupplyQuotas& msg ) const;
    //@}

private:
    const tools::Resolver_ABC< Formation_ABC >& formations_;
    const tools::Resolver_ABC< Automat_ABC >& automats_;
    const kernel::LogisticLevel*   pLogisticLevel_;
    Formation_ABC* pSuperiorFormation_;
    Automat_ABC* pSuperieurAutomat_;
    tools::Resolver< DotationQuota > quotas_;
};

} // namespace dispatcher

#endif // __DISPATCHER_LOGISTIC_ENTITY_H__

