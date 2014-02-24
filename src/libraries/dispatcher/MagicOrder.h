 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrder_h_
#define __MagicOrder_h_

#include "SimpleEntity.h"

namespace sword
{
    class MagicOrder;
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
class MagicOrder : public SimpleEntity<>
                 , public kernel::Updatable_ABC< sword::MagicOrder >
{
public:
    explicit MagicOrder( const Model& model, const sword::MagicOrder& msg );
    virtual ~MagicOrder();

    void DoUpdate( const sword::MagicOrder& msg );
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

private:
    std::unique_ptr< sword::MagicOrder > order_;
};

}

#endif // __MagicOrder_h_
