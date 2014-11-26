// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Hierarchy_h_
#define __Hierarchy_h_

#include "tools/Path.h"
#include <boost/noncopyable.hpp>
#include <memory>
#include <set>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace tools
{
    class Path;
    struct Sql_ABC;
}

namespace plugins
{
// Hierarchies stores hierarchies between entities
// in a persistent database and send updated state
// on authenticated clients
class Hierarchies : private boost::noncopyable
{
public:
             Hierarchies( const tools::Path& root, const tools::Path& filename );
    virtual ~Hierarchies();

    void SetParent( uint32_t entity, uint32_t parent );
    void SetTick( uint32_t tick );
    void Save( const tools::Path& filename );
    void SendState( dispatcher::ClientPublisher_ABC& );

    virtual void RegisterClient( dispatcher::ClientPublisher_ABC& );
    virtual void UnregisterClient( dispatcher::ClientPublisher_ABC& );

private:
    const tools::Path root_; // database root file path
    std::unique_ptr< tools::Sql_ABC > database_; // database handle
    std::set< dispatcher::ClientPublisher_ABC* > clients_; // authenticated clients
    uint32_t tick_; // current tick
};

}

#endif // __Hierarchy_h_
