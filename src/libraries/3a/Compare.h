// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Compare_h_
#define __Compare_h_

#include "Functions.h"
#include "TypeChecks.h"
#include <xeumeuleu/xml.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>

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
    {}
    virtual ~Compare() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        next_.BeginTick();
    };
    virtual void SetKey( const K& key )
    {
        next_.SetKey( key );
    };
    virtual void Apply( const T& lhs, const T& rhs )
    {
        next_.Apply( operator_( lhs, rhs ) );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Compare( const Compare& );            //!< Copy constructor
    Compare& operator=( const Compare& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef boost::function2< bool, T, T > T_Operator;
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Compare"; }
    static T_Operator MakeOperator( xml::xistream& xis )
    {
        const std::string op = xis.attribute< std::string >( "operator" );
        namespace bl = boost::lambda;
        if( op == "less" )          return bl::_1 < bl::_2;
        if( op == "greater" )       return bl::_1 > bl::_2;
        if( op == "less_equal" )    return bl::_1 <= bl::_2;
        if( op == "greater_equal" ) return bl::_1 >= bl::_2;
        if( op == "equal" )         return bl::_1 == bl::_2;
        if( op == "not_equal" )     return bl::_1 != bl::_2;
        xis.error( "Invalid operator '" + op + "'" );
        return T_Operator();
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
