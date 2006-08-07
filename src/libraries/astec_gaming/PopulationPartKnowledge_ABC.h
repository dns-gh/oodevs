// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPartKnowledge_ABC_h_
#define __PopulationPartKnowledge_ABC_h_

class Displayer_ABC;

// =============================================================================
/** @class  PopulationPartKnowledge_ABC
    @brief  Population part knowledge definition
*/
// Created: AGE 2006-02-27
// =============================================================================
class PopulationPartKnowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPartKnowledge_ABC() {};
    virtual ~PopulationPartKnowledge_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInList( Displayer_ABC& displayer ) const = 0;
    //@}
};

#endif // __PopulationPartKnowledge_ABC_h_
