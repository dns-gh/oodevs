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
    explicit Derivate( Function1_ABC< K, T >& next )
                : next_( next ), currentKey_() {}
             Derivate( xml::xistream& , Function1_ABC< K, T >& next )
                : next_( next ), currentKey_() {}
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
        T& previous = previous_[ currentKey_ ];
        next_.Apply( arg - previous );
        previous = arg;
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
    Function1_ABC< K, T >& next_;
    std::map< K, T > previous_;
    K currentKey_;
    //@}
};

#pragma warning( pop )

#endif // __Derivate_h_
