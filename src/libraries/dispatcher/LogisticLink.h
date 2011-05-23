// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DISPATCHER_LOGISTIC_LINK_H__
#define __DISPATCHER_LOGISTIC_LINK_H__

#include "tools/Resolver_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class SeqOfDotationQuota;
}

namespace dispatcher
{
    class LogisticHierarchyOwner_ABC;
    class LogisticEntity;
    class ClientPublisher_ABC;
    class DotationQuota;

// =============================================================================
/** @class  LogisticLink
    @brief  LogisticLink
*/
// Created: AHC 2010-10
// =============================================================================
class LogisticLink : private boost::noncopyable
{
public:
    //! @name Constructor/destructor
    //@{
     LogisticLink( const LogisticHierarchyOwner_ABC& owner, const LogisticEntity& superior );
    ~LogisticLink();
    //@}

    //! @name Operations
    //@{
    void Update        ( const sword::SeqOfDotationQuota& msg );
    void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Accessors
    //@{
    const LogisticEntity& GetSuperior() const { return superior_; }
    //@}

private:
    //! @name Member data
    //@{
    const LogisticHierarchyOwner_ABC& owner_;
    const LogisticEntity& superior_;
    tools::Resolver< DotationQuota > quotas_;
    //@}
};

} // namespace dispatcher

#endif // __DISPATCHER_LOGISTIC_LINK_H__
