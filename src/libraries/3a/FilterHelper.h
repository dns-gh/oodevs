// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FilterHelper_h_
#define __FilterHelper_h_

#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <set>

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
    FilterHelper()
    {
        // NOTHING
    }
    FilterHelper( xml::xistream& xis, const std::string& attribute )
    {
        FillAllowedValues( xis.attribute< std::string >( attribute, std::string() ) );
    }
    FilterHelper( xml::xistream& xis, const std::string& attribute, const std::string& variant )
    {
        std::string values = xis.attribute< std::string >( attribute, std::string() );
        if( values.empty() )
            values = xis.attribute< std::string >( variant, std::string() );
        FillAllowedValues( values );
    }
    virtual ~FilterHelper()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    bool IsAllowed( const T& value ) const
    {
        return allowed_.empty() || allowed_.find( value ) != allowed_.end();
    }
    //@}

private:
    void FillAllowedValues( const std::string& values )
    {
        static const boost::char_separator< char > separator( "," );
        const boost::tokenizer< boost::char_separator< char > > tokens( values, separator );
        std::transform( tokens.begin(), tokens.end(), std::inserter( allowed_, allowed_.begin() ), &boost::lexical_cast< T, std::string > );
    }

private:
    //! @name Member data
    //@{
    std::set< T > allowed_;
    //@}
};

#endif // __FilterHelper_h_
