// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Adder_h_
#define __Adder_h_

#include "Functions.h"
#include "ValueHandler_ABC.h"

// =============================================================================
/** @class  Adder
    @brief  Adder
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename K, typename T >
class Adder : public Function1_ABC< K, T >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Adder( ValueHandler_ABC< T >& handler ) 
        : handler_( handler ), sum_() {};
    virtual ~Adder() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Add"; }
    virtual void BeginTick()
    {
        sum_ = T();
    };
    virtual void SetKey( const K& )
    {
        // NOTHING
    };
    virtual void Apply( const T& value )
    {
        sum_ += value;
    };
    virtual void EndTick()
    {
        handler_.Handle( sum_ );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Adder( const Adder& );
    Adder& operator=( const Adder& );
    //@}

private:
    //! @name Member data
    //@{
    ValueHandler_ABC< T >& handler_;
    T sum_;
    //@}
};

#endif // __Adder_h_
