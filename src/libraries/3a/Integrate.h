// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Integrate_h_
#define __Integrate_h_

#include "Functions.h"
#include "TypeChecks.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  Integrate
    @brief  Integrate
*/
// Created: AGE 2007-09-25
// =============================================================================
template< typename K, typename T >
class Integrate : public Function1_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Integrate( Function1_ABC< K, T >& next )
                : next_( next ), sum_() {}
             Integrate( xml::xistream& , Function1_ABC< K, T >& next )
                : next_( next ), sum_() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    }
    virtual void Apply( const T& arg )
    {
        sum_ += arg;
        next_.Apply( sum_ );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Integrate( const Integrate& );            //!< Copy constructor
    Integrate& operator=( const Integrate& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& next_;
    T sum_;
    //@}
};

#endif // __Integrate_h_
