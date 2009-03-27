// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RangeHelper_h_
#define __RangeHelper_h_

#pragma warning (push)
#pragma warning (disable : 4100 4127 4511 4512 4702 )
#include <set>
#include <map>
#include <xeumeuleu/xml.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning (pop)

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  RangeHelper
    @brief  RangeHelper
*/
// Created: AHC 2008-08-04
// =============================================================================
template< typename T >
class RangeHelper
{

public:
    //! @name Constructors/Destructor
    //@{
    RangeHelper() {};
	// value of attribute must be '[min->max]' or 'value' or a sequence of these separated by ','
    RangeHelper( xml::xistream& xis, const std::string& attribute )
    {
        const std::string values = xml::attribute( xis, attribute, std::string() );
        if( ! values.empty() )
        {
            std::vector< std::string > split;
            boost::algorithm::split( split, values, boost::algorithm::is_any_of( "," ) );
			std::for_each( split.begin(), split.end(), boost::bind( &RangeHelper< T >::processRange, this, _1 ) );
        }
    }
    //@}

    //! @name Operations
    //@{
    bool IsAllowed( const T& value ) const
    {
        bool retval = ( simpleValues_.empty() && ranges_.empty() ) ||
			( !simpleValues_.empty() && ( simpleValues_.find( value ) != simpleValues_.end() ) ) ||
			std::find_if( ranges_.begin(), ranges_.upper_bound( value ), FindPred( value ) ) != ranges_.upper_bound( value );
		return retval;
    }
    //@}

private:
	struct FindPred
	{
		FindPred( const T& val ) : val_ ( val ) {}		
		bool operator()( std::pair<T, T> it )
		{
			return it.first <= val_ && it.second >= val_;
		}
		T val_;
	};

	void processRange( const std::string& range )
	{
		std::string rt = range;
		boost::algorithm::trim( rt );
		std::string::size_type pos;
		if( std::string::npos != ( pos = rt.find( "->" ) ) )
		{
			std::string a1 = rt.substr( 1, pos - 1 );
			std::string a2 = rt.substr( pos + 2, rt.length() - pos - 3 );
			ranges_.insert( std::make_pair< T, T >( boost::lexical_cast< T, std::string >( a1 ),
												    boost::lexical_cast< T, std::string >( a2 ) ) );
		}
		else
			simpleValues_.insert( boost::lexical_cast< T, std::string >( rt ) );
	}
    //! @name Member data
    //@{
    std::set< T > simpleValues_;
	std::map< T, T > ranges_;
	//@}
};

#endif // __RangeHelper_h_
