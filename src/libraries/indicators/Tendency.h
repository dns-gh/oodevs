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

#include <deque>

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
    explicit Tendency( unsigned int interval = 10 );
    virtual ~Tendency();
    //@}

    //! @name Operations
    //@{
    void AddValue( double value );
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Tendency( const Tendency& );            //!< Copy constructor
    Tendency& operator=( const Tendency& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int interval_;
    std::deque< double > values_;
    //@}
};

}

#endif // __Tendency_h_
