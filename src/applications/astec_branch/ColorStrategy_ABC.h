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

class Agent;
class Object;
class Population;

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
    virtual void SelectColor( const Agent& agent ) = 0;
    virtual void SelectColor( const Object& object ) = 0;
    virtual void SelectColor( const Population& population ) = 0;
    //@}
};

#endif // __ColorStrategy_ABC_h_
