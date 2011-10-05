// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteHlaObjectFactory_h
#define plugins_hla_RemoteHlaObjectFactory_h

#include "RemoteHlaObjectFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteHlaObjectFactory
    @brief  Remote hla object factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteHlaObjectFactory : public RemoteHlaObjectFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteHlaObjectFactory();
    virtual ~RemoteHlaObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > CreateAggregate( const std::string& name, RemoteAgentListener_ABC& listener ) const;
    //@}
};

}
}

#endif // plugins_hla_RemoteHlaObjectFactory_h
