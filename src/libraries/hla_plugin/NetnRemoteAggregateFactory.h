// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRemoteAggregateFactory_h
#define plugins_hla_NetnRemoteAggregateFactory_h

#include "RemoteAggregateFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnRemoteAggregateFactory
    @brief  Netn remote aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnRemoteAggregateFactory : public RemoteAggregateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnRemoteAggregateFactory( const RemoteAggregateFactory_ABC& factory );
    virtual ~NetnRemoteAggregateFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const;
    //@}

private:
    //! @name Member data
    //@{
    const RemoteAggregateFactory_ABC& factory_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteAggregateFactory_h
