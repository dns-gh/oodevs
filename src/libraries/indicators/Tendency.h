// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Tendency_h_
#define __Tendency_h_

namespace kernel
{
    class Displayer_ABC;
}

namespace indicators
{

// =============================================================================
/** @class  Tendency
    @brief  Tendency
*/
// Created: SBO 2009-05-05
// =============================================================================
class Tendency
{

public:
    //! @name Constructors/Destructor
    //@{
             Tendency();
    virtual ~Tendency();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer, double value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Tendency( const Tendency& );            //!< Copy constructor
    Tendency& operator=( const Tendency& ); //!< Assignment operator
    //@}
};

}

#endif // __Tendency_h_
