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

#include "Reductor_ABC.h"
#include "TypeChecks.h"
#pragma warning( push )
#pragma warning( disable : 4800 4804 )

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Adder
    @brief  Adder
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename K, typename T >
class Adder : public Reductor_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Adder( xml::xistream&, Function1_ABC< K, T >& handler )
        : handler_( handler )
        , sum_    ()
    {
        // NOTHING
    }
    explicit Adder( Function1_ABC< K, T >& handler )
        : handler_( handler )
        , sum_()
    {
        // NOTHING
    }
    virtual ~Adder()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Add"; }
    virtual void OnBeginTick()
    {
        sum_ = T();
        handler_.BeginTick();
    };
    virtual void Apply( const T& value )
    {
        sum_ += value;
    };
    virtual void OnEndTick()
    {
        handler_.Apply( sum_ );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& handler_;
    T sum_;
    //@}
};

#pragma warning( pop )

#endif // __Adder_h_
