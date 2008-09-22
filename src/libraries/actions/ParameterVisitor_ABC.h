// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterVisitor_ABC_h_
#define __ActionParameterVisitor_ABC_h_

namespace actions
{
    namespace parameters {
        class Numeric;
        class Obstacle;
        class Objective;
        class Lima;
        class Intelligence;
        class Location;
        class Path;
        class PathPoint;
        class Agent;
        class Automat;
        class AgentKnowledge;
        class ObjectKnowledge;
        class PopulationKnowledge;
        class DotationType;
    }

// =============================================================================
/** @class  ParameterVisitor_ABC
    @brief  ParameterVisitor_ABC
*/
// Created: SBO 2007-05-22
// =============================================================================
class ParameterVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParameterVisitor_ABC() {}
    virtual ~ParameterVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const parameters::Numeric&             ) {}
    virtual void Visit( const parameters::Obstacle&            ) {}
    virtual void Visit( const parameters::Objective&           ) {}
    virtual void Visit( const parameters::Lima&                ) {}
    virtual void Visit( const parameters::Intelligence&        ) {}
    virtual void Visit( const parameters::Path&                ) {}
    virtual void Visit( const parameters::PathPoint&           ) {}
    virtual void Visit( const parameters::Location&            ) {}
    virtual void Visit( const parameters::Agent&               ) {}
    virtual void Visit( const parameters::Automat&             ) {}
    virtual void Visit( const parameters::AgentKnowledge&      ) {}
    virtual void Visit( const parameters::ObjectKnowledge&     ) {}
    virtual void Visit( const parameters::PopulationKnowledge& ) {}
    virtual void Visit( const parameters::DotationType&        ) {}
    //@}
};

}

#endif // __ActionParameterVisitor_ABC_h_
