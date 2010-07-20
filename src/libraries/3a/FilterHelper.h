// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FilterHelper_h_
#define __FilterHelper_h_

#pragma warning( push, 0 )
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <set>
#pragma warning( pop )

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FilterHelper
    @brief  FilterHelper
*/
// Created: AGE 2007-10-29
// =============================================================================
template< typename T >
class FilterHelper
{
public:
    //! @name Constructors/Destructor
    //@{
    FilterHelper() {};
    FilterHelper( xml::xistream& xis, const std::string& attribute )
    {
        const std::string values = xis.attribute< std::string >( attribute, std::string() );
        if( ! values.empty() )
        {
            std::vector< std::string > split;
            boost::algorithm::split( split, values, boost::algorithm::is_any_of( "," ) );
            std::transform( split.begin(), split.end(), std::inserter( allowed_, allowed_.begin() ), &boost::lexical_cast< T, std::string > );
        }
    }
    virtual ~FilterHelper() {};
    //@}

    //! @name Operations
    //@{
    bool IsAllowed( const T& value ) const
    {
        return allowed_.empty() || allowed_.find( value ) != allowed_.end();
    }
    //@}

private:
    //! @name Member data
    //@{
    std::set< T > allowed_;
    //@}
};

#endif // __FilterHelper_h_
