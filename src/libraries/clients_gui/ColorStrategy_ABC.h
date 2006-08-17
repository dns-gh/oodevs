// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorStrategy_ABC_h_
#define __ColorStrategy_ABC_h_

class Agent_ABC;
class Object_ABC;
class Population_ABC;
class AgentKnowledge;
class ObjectKnowledge;
class PopulationKnowledge;
class TacticalLine_ABC;

// =============================================================================
/** @class  ColorStrategy_ABC
    @brief  Color selection strategy definition
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy_ABC() {};
    virtual ~ColorStrategy_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void SelectColor( const Agent_ABC& agent ) = 0;
    virtual void SelectColor( const Object_ABC& object ) = 0;
    virtual void SelectColor( const Population_ABC& population ) = 0;
    virtual void SelectColor( const AgentKnowledge& k ) = 0;
    virtual void SelectColor( const ObjectKnowledge& k ) = 0;
    virtual void SelectColor( const PopulationKnowledge& k ) = 0;
    virtual void SelectColor( const TacticalLine_ABC& line ) = 0;
    //@}
};

#endif // __ColorStrategy_ABC_h_
