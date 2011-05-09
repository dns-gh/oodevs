// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Meaner_h_
#define __Meaner_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"

#pragma warning( push )
#pragma warning( disable : 4800 4804 )

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Meaner
    @brief  Meaner
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename K, typename T >
class Meaner : public Reductor_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     Meaner( xml::xistream&, Function1_ABC< K, T >& handler )
        : handler_( handler )
        , sum_    ()
        , count_  ( 0 )
     {
         // NOTHING
     }
    explicit Meaner( Function1_ABC< K, T >& handler )
        : handler_( handler )
        , sum_    ()
        , count_  ( 0 )
    {
        // NOTHING
    }
    virtual ~Meaner()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Mean"; }
    virtual void OnBeginTick()
    {
        sum_ = T();
        count_ = 0;
        handler_.BeginTick();
    };
    virtual void Apply( const T& value )
    {
        sum_ += value;
        ++count_;
    }
    virtual void OnEndTick()
    {
        if( count_ )
            handler_.Apply( sum_ / T( count_ ) );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& handler_;
    T sum_;
    unsigned int count_;
    //@}
};

#pragma warning( pop )

#endif // __Meaner_h_
