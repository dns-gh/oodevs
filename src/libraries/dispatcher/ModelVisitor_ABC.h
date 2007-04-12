// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelVisitor_ABC_h_
#define __ModelVisitor_ABC_h_

namespace dispatcher
{
    class Side;
    class KnowledgeGroup;
    class Formation;
    class Automat;
    class Agent;
    class Object;
    class Population;
    class PopulationConcentration;
    class PopulationFlow;

// =============================================================================
/** @class  ModelVisitor_ABC
    @brief  ModelVisitor_ABC
*/
// Created: AGE 2007-04-12
// =============================================================================
class ModelVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ModelVisitor_ABC() {};
    virtual ~ModelVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Visit( Side& entity ) = 0;
    virtual void Visit( KnowledgeGroup& entity ) = 0;
    virtual void Visit( Formation& entity ) = 0;
    virtual void Visit( Automat& entity ) = 0;
    virtual void Visit( Agent& entity ) = 0;
    virtual void Visit( Object& entity ) = 0;
    virtual void Visit( Population& entity ) = 0;
    virtual void Visit( PopulationConcentration& entity ) = 0;
    virtual void Visit( PopulationFlow& entity ) = 0;
    //@}
};

}

#endif // __ModelVisitor_ABC_h_
