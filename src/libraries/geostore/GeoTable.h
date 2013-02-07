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

#include "Table.h"
#include <vector>

class TerrainObject;
class PointProjector_ABC;

namespace geostore
{
// =============================================================================
/** @class  GeoTable
    @brief  GeoTable
*/
// Created: AME 2010-07-19
// =============================================================================
class GeoTable : public Table
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
    virtual ~GeoTable();
    //@}

    //! @name Operations
    //@{
    void LoadTable();
    void FillTable( int geomType, const std::vector< TerrainObject* >& features );
    void FillTable( const boost::filesystem::path& path, PointProjector_ABC& proj );

    GeometryType GetGeometryType() const;
    gaiaGeomCollPtr GetFeaturesIntersectingWith( gaiaGeomCollPtr poly ) const;
    //@}

private:
    //! @name Helpers
    //@{
    std::vector< gaiaGeomCollPtr > CreatePolygonGeometry( const TerrainObject& shape );
    std::vector< gaiaGeomCollPtr > CreateLineGeometry( const TerrainObject& shape );
    std::vector< gaiaGeomCollPtr > CreateGeometry( const TerrainObject& shape );
    void SetGeometry( const std::string& name );
    void Fill( const std::vector< TerrainObject* >& features );
    void AddGeometryColumn( int geom_type );
    //@}

private:
    //! @name Member data
    //@{
    GeometryType geometryType_;
    //@}
};

}

#endif // __GeoTable_h_
