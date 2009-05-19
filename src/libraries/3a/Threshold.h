// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Threshold_h_
#define __Threshold_h_

#include "Reductor_ABC.h"
#include "TypeChecks.h"
#include <set>
#include <map>
#include <xeumeuleu/xml.h>
#pragma warning( push )
#pragma warning( disable : 4800 4804 4996 )
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

namespace xml { class xistream; }

// =============================================================================
/** @class  Threshold
    @brief  Threshold
*/
// Created: SBO 2009-03-12
// =============================================================================
template< typename K, typename T >
class Threshold : public Reductor_ABC< K, T >, private types::Arithmetic< T >
{
public:
    //! @name Types
    //@{
    typedef T Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Threshold( xml::xistream& xis, Function1_ABC< K, T >& handler )
                : handler_( handler )
                , thresholds_( ReadThresholds( xis ) ) {};
    virtual ~Threshold() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Threshold"; }
    virtual void OnBeginTick()
    {
        handler_.BeginTick();
    };
    virtual void SetKey( const K& /*key*/ )
    {
    }
    virtual void Apply( const T& value )
    {
        handler_.Apply( ApplyThreshold( value ) );
    };
    virtual void OnEndTick()
    {
        handler_.EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Threshold( const Threshold& );            //!< Copy constructor
    Threshold& operator=( const Threshold& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::map< double, double > ReadThresholds( xml::xistream& xis )
    {
        std::vector< std::string > split;
        std::string list( xml::attribute< std::string >( xis, "thresholds" ) );
        boost::algorithm::split( split, list, boost::algorithm::is_any_of( "," ) );
        std::set< double > ranges;
        for( std::vector< std::string >::const_iterator it = split.begin(); it != split.end(); ++it )
            ranges.insert( boost::lexical_cast< double, std::string >( *it ) );
        ranges.insert( std::numeric_limits< double >::max() );
        
        list = xml::attribute< std::string >( xis, "values" );
        boost::algorithm::split( split, list, boost::algorithm::is_any_of( "," ) );
        if( split.size() < ranges.size() )
        {
            const std::string lastValue = split.empty() ? "0" : split.back();
            std::fill_n( std::back_inserter( split ), ranges.size() - split.size(), lastValue );
        }
       
        std::map< double, double > thresholds;
        std::vector< std::string >::iterator valuesIt = split.begin();
        for( std::set< double >::const_iterator it = ranges.begin(); it != ranges.end(); ++it )
            thresholds.insert( std::make_pair( *it, boost::lexical_cast< double, std::string >( *valuesIt++ ) ) );
        return thresholds;
    }

    T ApplyThreshold( const T& value )
    {
        std::map< double, double >::const_iterator it = thresholds_.lower_bound( double( value ) );
        return it != thresholds_.end() ? T( it->second ) : T();
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, T >& handler_;
    const std::map< double, double > thresholds_;
    //@}
};

#endif // __Threshold_h_
