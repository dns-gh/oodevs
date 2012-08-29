// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Visitors_h_
#define __Visitors_h_

#include "clients_kernel/ModelVisitor_ABC.h"
#include <set>

namespace dispatcher
{
    class ClientPublisher_ABC;

    class FullUpdateVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        FullUpdateVisitor( ClientPublisher_ABC& publisher );
        virtual ~FullUpdateVisitor() {}
        virtual void Visit( const kernel::EntityBase_ABC& entity );
    private:
        ClientPublisher_ABC* publisher_;
    };

    class CreationVisitor : public kernel::ModelVisitor_ABC
    {
    public:
        CreationVisitor( ClientPublisher_ABC& publisher );
        virtual ~CreationVisitor() {}
        virtual void Visit( const kernel::EntityBase_ABC& entity );
    private:
        ClientPublisher_ABC* publisher_;
        std::set< const kernel::EntityBase_ABC* > entities_;
    };
}

#endif // __Visitors_h_
