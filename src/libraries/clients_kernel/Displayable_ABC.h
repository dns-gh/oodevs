// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Displayable_ABC_h_
#define __Displayable_ABC_h_

namespace kernel
{
    class Displayer_ABC;

// =============================================================================
/** @class  Displayable_ABC
    @brief  Displayable_ABC
*/
// Created: AGE 2006-08-24
// =============================================================================
class Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Displayable_ABC() {};
    virtual ~Displayable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Display         ( Displayer_ABC& ) const {};
    virtual void DisplayInList   ( Displayer_ABC& ) const {};
    virtual void DisplayInTooltip( Displayer_ABC& ) const {};
    //@}
};

}

#endif // __Displayable_ABC_h_
