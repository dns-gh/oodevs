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

class ActionParameterNumeric;
class ActionParameterObstacle;
class ActionParameterObjective;
class ActionParameterLima;
class ActionParameterIntelligence;
class ActionParameterLocation;
class ActionParameterPath;
class ActionParameterPathPoint;
class ActionParameterAgent;
class ActionParameterAutomat;
class ActionParameterAgentKnowledge;
class ActionParameterObjectKnowledge;
class ActionParameterPopulationKnowledge;
class ActionParameterDotationType;

// =============================================================================
/** @class  ActionParameterVisitor_ABC
    @brief  ActionParameterVisitor_ABC
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterVisitor_ABC() {}
    virtual ~ActionParameterVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const ActionParameterNumeric&             ) {}
    virtual void Visit( const ActionParameterObstacle&            ) {}
    virtual void Visit( const ActionParameterObjective&           ) {}
    virtual void Visit( const ActionParameterLima&                ) {}
    virtual void Visit( const ActionParameterIntelligence&        ) {}
    virtual void Visit( const ActionParameterPath&                ) {}
    virtual void Visit( const ActionParameterPathPoint&           ) {}
    virtual void Visit( const ActionParameterLocation&            ) {}
    virtual void Visit( const ActionParameterAgent&               ) {}
    virtual void Visit( const ActionParameterAutomat&             ) {}
    virtual void Visit( const ActionParameterAgentKnowledge&      ) {}
    virtual void Visit( const ActionParameterObjectKnowledge&     ) {}
    virtual void Visit( const ActionParameterPopulationKnowledge& ) {}
    virtual void Visit( const ActionParameterDotationType& )        {}
    //@}
};

#endif // __ActionParameterVisitor_ABC_h_
