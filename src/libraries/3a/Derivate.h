// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Derivate_h_
#define __Derivate_h_

#include "Functions.h"
#include "TypeChecks.h"
#include <map>
#include <deque>
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable : 4800 4804 )

namespace xml { class xistream; }

// =============================================================================
/** @class  Derivate
    @brief  Derivate
*/
// Created: AGE 2007-09-25
// =============================================================================
template< typename K, typename T >
class Derivate : public Function1_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Derivate( xml::xistream& xis, Function1_ABC< K, T >& next )
        : period_( double( std::max( xml::attribute< unsigned int >( xis, "period", 1 ), 1u ) ) ), next_( next ), currentKey_() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    }
    virtual void SetKey( const K& key )
    {
        currentKey_ = key;
        next_.SetKey( key );
    }
    virtual void Apply( const T& arg )
    {
        std::deque< T >& previous = previous_[ currentKey_ ];
        next_.Apply( ( arg - ( previous.empty() ? 0 : previous.back() ) ) / period_ );
        previous.push_front( arg );
        if( previous.size() > period_ )
            previous.pop_back();
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Derivate( const Derivate& );            //!< Copy constructor
    Derivate& operator=( const Derivate& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Derivate"; }
    //@}

private:
    //! @name Member data
    //@{
    NumericValue period_;
    Function1_ABC< K, T >& next_;
    std::map< K, std::deque< T > > previous_;
    K currentKey_;
    //@}
};

#pragma warning( pop )

#endif // __Derivate_h_
