 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Action_h_
#define __Action_h_

#include "SimpleEntity.h"

namespace sword
{
    class Action;
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
class Action : public SimpleEntity<>
                 , public kernel::Updatable_ABC< sword::Action >
{
public:
             Action( const Model& model, const sword::Action& msg );
    virtual ~Action();

    void DoUpdate( const sword::Action& msg );
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

private:
    std::unique_ptr< sword::Action > action_;
};

}

#endif // __Action_h_
