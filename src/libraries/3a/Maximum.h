// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Maximum_h_
#define __Maximum_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Maximum
    @brief  Maximum
*/
// Created: AGE 2007-09-25
// =============================================================================
template< typename K, typename T >
class Maximum : public Reductor_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Maximum( Function1_ABC< K, T >& handler )
        : handler_( handler )
        , max_    ( std::numeric_limits< T >::min() )
    {
        // NOTHING
    }
    Maximum( xml::xistream&, Function1_ABC< K, T >& handler )
        : handler_( handler )
        , max_    ( std::numeric_limits< T >::min() )
    {
        // NOTHING
    }
    virtual ~Maximum()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Maximum"; }
    virtual void OnBeginTick()
    {
        max_ = std::numeric_limits< T >::min();
        handler_.BeginTick();
    }
    virtual void Apply( const T& value )
    {
        max_ = std::max( value, max_ );
    }
    virtual void OnEndTick()
    {
        handler_.Apply( max_ );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& handler_;
    T max_;
    //@}
};

#endif // __Maximum_h_
