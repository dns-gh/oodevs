// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Domain_h_
#define __Domain_h_

#include "Functions.h"
#include <xeumeuleu/xml.h>
#include <vector>
#pragma warning (push)
#pragma warning (disable : 4100 4127 4511 4512 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// =============================================================================
/** @class  Domain
    @brief  Domain
*/
// Created: AGE 2007-10-03
// =============================================================================
template< typename K, typename T >
class Domain : public Function1_ABC< K, T >
{
public:
    //! @name 
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Domain( xml::xistream& xis, Function1_ABC< K, T >& next )
        : next_    ( next )
        , selected_( false )
    {
        const std::string values = xml::attribute< std::string >( xis, "select" );
        if( !values.empty() )
        {
            std::vector< std::string > split;
            boost::algorithm::split( split, values, boost::algorithm::is_any_of( "," ) );
            std::transform( split.begin(), split.end(), std::back_inserter( allowed_ ), &boost::lexical_cast< K, std::string > );
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
        selected_ = allowed_.empty() || std::find( allowed_.begin(), allowed_.end(), key ) != allowed_.end();
        if( selected_ )
            next_.SetKey( key );
    }
    virtual void Apply( const T& arg )
    {
        if( selected_ )
            next_.Apply( arg );
        selected_ = false;
    }
    virtual void EndTick()
    {
        next_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Domain( const Domain& );            //!< Copy constructor
    Domain& operator=( const Domain& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Domain"; }
    //@}

    //! @name Types
    //@{
    typedef std::vector< K > T_Keys;
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& next_;
    T_Keys allowed_;
    bool selected_;
    //@}
};

#endif // __Domain_h_
