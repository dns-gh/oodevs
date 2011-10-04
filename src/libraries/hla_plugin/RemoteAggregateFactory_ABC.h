// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAggregateFactory_ABC_h
#define plugins_hla_RemoteAggregateFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>

namespace plugins
{
namespace hla
{
    class HlaObject_ABC;
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  RemoteAggregateFactory_ABC
    @brief  RemoteAggregate factory definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class RemoteAggregateFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAggregateFactory_ABC() {}
    virtual ~RemoteAggregateFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< HlaObject_ABC > Create( const std::string& name, RemoteAgentListener_ABC& listener ) const = 0;
    //@}
};

}
}

#endif // __RemoteAggregateFactory_ABC_h_
