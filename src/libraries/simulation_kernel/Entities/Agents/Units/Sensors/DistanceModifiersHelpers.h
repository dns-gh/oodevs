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
    std::map< std::string, double > ReadDistanceModifiers( xml::xistream& xis, const std::string& parent );
    void ReadPostureFactors( xml::xistream& xis, const std::string& parent, std::vector< double >& factors );
    void InitializeTerrainModifiers( xml::xistream& xis, std::map< unsigned int, double >& factors );
    void InitializeUrbanFactors( xml::xistream& xis, std::vector< double >& factors );
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

    bool ComputeUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ, const std::vector< double >& factors, bool posted );
}

#endif // __DistanceModifiersHelpers_h_
