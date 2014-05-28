 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Pathfind_h_
#define __Pathfind_h_

#include "SimpleEntity.h"

namespace sword
{
    class Pathfind;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Model;
}

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace dispatcher
{
class Pathfind : public SimpleEntity<>
               , public kernel::Updatable_ABC< sword::Pathfind >
{
public:
             Pathfind( const Model& model, const sword::Pathfind& msg );
    virtual ~Pathfind();

    void DoUpdate( const sword::Pathfind& msg );
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

private:
    std::unique_ptr< sword::Pathfind > data_;
};

}

#endif // __Pathfind_h_
