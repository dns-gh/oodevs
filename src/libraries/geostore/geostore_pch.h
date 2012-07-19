// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __geostore_pch_h_
#define __geostore_pch_h_

#pragma execution_character_set("utf-8")

#pragma warning ( push, 0 )
#pragma warning ( disable : 4100 4512 )
#pragma warning ( disable : 4290 )
#pragma warning ( disable : 4702 )

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#pragma warning ( pop )

#pragma warning ( push, 0 )
#pragma warning ( disable : 4127 )
#include <QtGui/qcolor.h>
#include <QtCore/qstring.h>
#pragma warning ( pop )

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

// these headers are required in order to support SQLite/SpatiaLite
#include <spatialite/spatialite/sqlite3.h>
#include <spatialite/spatialite/gaiageo.h>
#include <spatialite/spatialite.h>
#include <xeumeuleu/xml.hpp>

#include "geodata/Types.h"

#include <geometry/Types.h>
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#endif // __geostore_pch_h_