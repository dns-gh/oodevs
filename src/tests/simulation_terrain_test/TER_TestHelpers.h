// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <string>
#include <boost/shared_ptr.hpp>

class TER_Localisation;
class TER_Polygon;

// Parses expressions like:
//
//   POLYGON (5 6, 6 7, 7 8)
//
TER_Polygon FromWKT( std::string wkt, bool convexHull );

boost::shared_ptr< TER_Localisation > LocalisationFromWKT( const std::string& wkt );
