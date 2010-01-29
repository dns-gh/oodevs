// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Minimum_h_
#define __Minimum_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  Minimum
    @brief  Minimum
*/
// Created: AGE 2007-09-25
// =============================================================================
template< typename K, typename T >
class Minimum : public Reductor_ABC< K, T > , private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Minimum( Function1_ABC< K, T >& handler )
                 : handler_( handler ), min_( std::numeric_limits< T >::max() ) {};
             Minimum( xml::xistream&, Function1_ABC< K, T >& handler )
                 : handler_( handler ), min_( std::numeric_limits< T >::max() ) {};
    virtual ~Minimum() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Minimum"; }
    virtual void OnBeginTick()
    {
        min_ = std::numeric_limits< T >::max();
        handler_.BeginTick();
    }
    virtual void Apply( const T& value )
    {
        min_ = std::min( value, min_ );
    }
    virtual void OnEndTick()
    {
        handler_.Apply( min_ );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Minimum( const Minimum& );            //!< Copy constructor
    Minimum& operator=( const Minimum& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& handler_;
    T min_;
    //@}
};

#endif // __Minimum_h_
