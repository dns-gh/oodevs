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

#pragma warning( push )
#pragma warning( disable : 4800 4804 )

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
                : next_( next ), currentKey_() {}
             Integrate( xml::xistream& , Function1_ABC< K, T >& next )
                : next_( next ), currentKey_() {}
    virtual ~Integrate() {}
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
        T& sum = sum_[ currentKey_ ];
        sum += arg;
        next_.Apply( sum );
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

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Integrate"; }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& next_;
    std::map< K, T > sum_;
    K currentKey_;
    //@}
};

#pragma warning( pop )

#endif // __Integrate_h_
