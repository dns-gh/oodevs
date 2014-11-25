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
#include "tools/Sql_ABC.h"
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
}

namespace plugins
{

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
    const tools::Path root_;
    std::unique_ptr< tools::Sql_ABC > database_;
    std::set< dispatcher::ClientPublisher_ABC* > clients_;
    uint32_t tick_;
};

}

#endif // __Hierarchy_h_
