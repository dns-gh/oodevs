// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __geostore_pch_h_
#define __geostore_pch_h_

#pragma warning ( push, 0 )
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/noncopyable.hpp>
#pragma warning ( pop )
#include <sqlite/sqlite3.h>
#include <spatialite/gaiageo.h>
#include <spatialite.h>
#include <tools/Exception.h>
#include <geometry/Types.h>

#endif // __geostore_pch_h_
