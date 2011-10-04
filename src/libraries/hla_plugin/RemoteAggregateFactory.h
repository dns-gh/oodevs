// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAggregateFactory_h
#define plugins_hla_RemoteAggregateFactory_h

#include "RemoteAggregateFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAggregateFactory
    @brief  Remote aggregate factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteAggregateFactory : public RemoteAggregateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAggregateFactory();
    virtual ~RemoteAggregateFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name, RemoteAgentListener_ABC& listener ) const;
    //@}
};

}
}

#endif // plugins_hla_RemoteAggregateFactory_h
