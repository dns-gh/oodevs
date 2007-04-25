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

#include "ModelVisitor_ABC.h"

namespace dispatcher
{
    class Publisher_ABC;
    class Synchroniser;

    class FullUpdateVisitor : public ModelVisitor_ABC
    {
    public:
        FullUpdateVisitor( Publisher_ABC& publisher );
        virtual void Visit( Entity_ABC& entity );
    private:
        Publisher_ABC* publisher_;
    };

    class CreationVisitor : public ModelVisitor_ABC
    {
    public:
        CreationVisitor( Publisher_ABC& publisher );
        virtual void Visit( Entity_ABC& entity );
    private:
        Publisher_ABC* publisher_;
    };

    class StartSynchVisitor : public ModelVisitor_ABC
    {
    public:
        virtual void Visit( Entity_ABC& entity );
    };

    class EndSynchVisitor : public ModelVisitor_ABC
    {
    public:
        EndSynchVisitor( Synchroniser& model );
        virtual void Visit( Entity_ABC& entity );
    private:
        Synchroniser* model_;
    };
}

#endif // __Visitors_h_
