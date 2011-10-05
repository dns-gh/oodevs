// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnRemoteHlaObjectFactory_h
#define plugins_hla_NetnRemoteHlaObjectFactory_h

#include "RemoteHlaObjectFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnRemoteHlaObjectFactory
    @brief  Netn remote aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnRemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnRemoteHlaObjectFactory( const RemoteHlaObjectFactory_ABC& factory );
    virtual ~NetnRemoteHlaObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const;
    //@}

private:
    //! @name Member data
    //@{
    const RemoteHlaObjectFactory_ABC& factory_;
    //@}
};

}
}

#endif // plugins_hla_NetnRemoteHlaObjectFactory_h
