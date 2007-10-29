// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IsOneOf_h_
#define __IsOneOf_h_

#include "Functions.h"
#include "xeumeuleu/xml.h"
#include <vector>
#pragma warning (push)
#pragma warning (disable : 4100 4127 4511 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// =============================================================================
/** @class  IsOneOf
    @brief  IsOneOf
*/
// Created: AGE 2007-09-14
// =============================================================================
template< typename K, typename T >
class IsOneOf : public Function1_ABC< K, T >
{
public:
    //! @name Types
    //@{
    typedef bool Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    IsOneOf( xml::xistream& xis, Function1_ABC< K, bool >& next )
        : next_( next )
    {
        const std::string values = xml::attribute< std::string >( xis, "select" );
        if( !values.empty() )
        {
            std::vector< std::string > split;
            boost::algorithm::split( split, values, boost::algorithm::is_any_of( "," ) );
            std::transform( split.begin(), split.end(), std::back_inserter( allowed_ ), &boost::lexical_cast< T, std::string > );
        }
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
    virtual void Apply( const T& arg )
    {
        next_.Apply( allowed_.empty() || std::find( allowed_.begin(), allowed_.end(), arg ) != allowed_.end() );
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IsOneOf( const IsOneOf& );            //!< Copy constructor
    IsOneOf& operator=( const IsOneOf& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "IsOneOf"; }
    //@}

    //! @name Types
    //@{
    typedef std::vector< T > T_Values;
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, bool >& next_;
    T_Values allowed_;
    //@}
};

#pragma warning (pop)

#endif // __IsOneOf_h_
