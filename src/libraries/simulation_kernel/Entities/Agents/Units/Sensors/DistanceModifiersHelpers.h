// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DistanceModifiersHelpers_h_
#define __DistanceModifiersHelpers_h_

namespace distance_modifiers
{
    // Extract "type" and "value" attribute of "distance-modifier" child elements
    // of "parent" in xis. Values must be double within [0, 1], an exception is
    // thrown otherwise.
    std::map< std::string, double > ReadDistanceModifiers( xml::xistream& xis, const std::string& parent );

    // Reads posture elements, maps their type to an integer identifier and sets
    // factors[ id ] to the posture factor. "factors" size must be greater or
    // equal to PHY_Posture::GetPostureCount().
    void ReadPostureFactors( xml::xistream& xis, const std::string& parent, std::vector< double >& factors );

    template< typename C >
    void InitializeFactors( const C& container, const std::string& strTagName, std::vector< double >& factors, xml::xistream& xis )
    {
        auto values = ReadDistanceModifiers( xis, strTagName );
        for( auto it = values.cbegin(); it != values.cend(); ++it )
        {
            auto ic = container.find( it->first );
            if( ic == container.end() )
                throw MASA_EXCEPTION( xis.context() + "distance-modifier: unknown type: " + it->first );
            factors[ ic->second->GetID() ] = it->second;
        }
    }

}

#endif // __DistanceModifiersHelpers_h_
