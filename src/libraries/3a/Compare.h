// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Compare_h_
#define __Compare_h_

#include "Functions.h"
#include "TypeChecks.h"
#include <xeumeuleu/xml.hpp>

// =============================================================================
/** @class  Compare
    @brief  Compare
*/
// Created: AGE 2007-10-16
// =============================================================================
template< typename K, typename T >
class Compare : public Function2_ABC< K, T, T >, private types::Arithmetic< T >
{
public:
    //! @name
    //@{
    typedef bool Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Compare( xml::xistream& xis, Function1_ABC< K, Result_Type >& next )
        : operator_( MakeOperator( xis ) )
        , next_    ( next )
    {
        // NOTHING
    }
    virtual ~Compare()
    {
        // NOTHING
    }
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
    virtual void Apply( const T& lhs, const T& rhs )
    {
        next_.Apply( operator_( lhs, rhs ) );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::function< bool( T, T ) > T_Operator;
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const
    {
        return "Compare";
    }
    static T_Operator MakeOperator( xml::xistream& xis )
    {
        const std::string op = xis.attribute< std::string >( "operator" );
        if( op == "less" )          return []( T a, T b ){ return a < b; };
        if( op == "greater" )       return []( T a, T b ){ return a > b; };
        if( op == "less_equal" )    return []( T a, T b ){ return a <= b; };
        if( op == "greater_equal" ) return []( T a, T b ){ return a >= b; };
        if( op == "equal" )         return []( T a, T b ){ return a == b; };
        if( op == "not_equal" )     return []( T a, T b ){ return a != b; };
        throw MASA_EXCEPTION( xis.context() + "Invalid operator '" + op + "'" );
    }
    //@}

private:
    //! @name Member data
    //@{
    T_Operator operator_;
    Function1_ABC< K, Result_Type >& next_;
    //@}
};

#endif // __Compare_h_
