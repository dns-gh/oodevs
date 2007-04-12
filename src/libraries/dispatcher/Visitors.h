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
    class Model;

    class FullUpdateVisitor : public ModelVisitor_ABC
    {
    public:
        FullUpdateVisitor( Publisher_ABC& publisher );
        virtual void Visit( Side& entity );
        virtual void Visit( KnowledgeGroup& entity );
        virtual void Visit( Formation& entity );
        virtual void Visit( Automat& entity );
        virtual void Visit( Agent& entity );
        virtual void Visit( Object& entity );
        virtual void Visit( Population& entity );
        virtual void Visit( PopulationFlow& entity );
        virtual void Visit( PopulationConcentration& entity );
    private:
        Publisher_ABC* publisher_;
    };

    class CreationVisitor : public ModelVisitor_ABC
    {
    public:
        CreationVisitor( Publisher_ABC& publisher );
        virtual void Visit( Side& entity );
        virtual void Visit( KnowledgeGroup& entity );
        virtual void Visit( Formation& entity );
        virtual void Visit( Automat& entity );
        virtual void Visit( Agent& entity );
        virtual void Visit( Object& entity );
        virtual void Visit( Population& entity );
        virtual void Visit( PopulationFlow& entity );
        virtual void Visit( PopulationConcentration& entity );
    private:
        Publisher_ABC* publisher_;
    };

    class StartSynchVisitor : public ModelVisitor_ABC
    {
    public:
        StartSynchVisitor( Publisher_ABC& publisher );
        virtual void Visit( Side& entity );
        virtual void Visit( KnowledgeGroup& entity );
        virtual void Visit( Formation& entity );
        virtual void Visit( Automat& entity );
        virtual void Visit( Agent& entity );
        virtual void Visit( Object& entity );
        virtual void Visit( Population& entity );
        virtual void Visit( PopulationConcentration& entity );
        virtual void Visit( PopulationFlow& entity );
    private:
        Publisher_ABC* publisher_;
    };

    class EndSynchVisitor : public ModelVisitor_ABC
    {
    public:
        EndSynchVisitor( Model& model );
        virtual void Visit( Side& entity );
        virtual void Visit( KnowledgeGroup& entity );
        virtual void Visit( Formation& entity );
        virtual void Visit( Automat& entity );
        virtual void Visit( Agent& entity );
        virtual void Visit( Object& entity );
        virtual void Visit( Population& entity );
        virtual void Visit( PopulationConcentration& entity );
        virtual void Visit( PopulationFlow& entity );
    private:
        Model* model_;
    };
}

#endif // __Visitors_h_
