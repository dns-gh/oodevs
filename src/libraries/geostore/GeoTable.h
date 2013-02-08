// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __GeoTable_h_
#define __GeoTable_h_

#include "GeometryCollection.h"
#include "Table.h"
#include <vector>

class TerrainObject;

namespace geostore
{
// =============================================================================
/** @class  GeoTable
    @brief  GeoTable
*/
// Created: AME 2010-07-19
// =============================================================================
class GeoTable : private Table
{
public:
    enum GeometryType
    {
        Point       = 0,
        Polygon     = 1,
        LineString  = 2,
    };

    //! @name Constructors/Destructor
    //@{
    GeoTable( sqlite3* db, const std::string& name );
    GeoTable( sqlite3* db, const std::string& name, int geomType, const std::vector< TerrainObject* >& features );
    //@}

    //! @name Operations
    //@{
    GeometryType GetGeometryType() const;
    GeometryCollection GetFeaturesIntersectingWith( GeometryCollection poly ) const;
    //@}

private:
    //! @name Helpers
    //@{
    std::vector< GeometryCollection > CreatePolygonGeometry( const TerrainObject& shape );
    std::vector< GeometryCollection > CreateLineGeometry( const TerrainObject& shape );
    std::vector< GeometryCollection > CreateGeometry( const TerrainObject& shape );
    void SetGeometryType( const std::string& name );
    void Fill( const std::vector< TerrainObject* >& features );
    void AddGeometryColumn( int geomType );
    //@}

private:
    //! @name Member data
    //@{
    GeometryType geometryType_;
    //@}
};

}

#endif // __GeoTable_h_
