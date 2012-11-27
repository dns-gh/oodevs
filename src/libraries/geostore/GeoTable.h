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

class TerrainObject;

namespace geostore
{

// =============================================================================
/** @class  GeoTable
    @brief  GeoTable
*/
// Created: AME 2010-07-19
// =============================================================================
class GeoTable : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GeoTable( sqlite3* db, const std::string& name );
    virtual ~GeoTable();
    //@}

    //! @name Operations
    //@{
    void Fill( std::vector< TerrainObject* > features );
    void AddGeometryColumn( int geom_type );
    gaiaGeomCollPtr GetFeaturesIntersectsWith( gaiaGeomCollPtr poly );
    const std::string& GetName() const;
    const std::string& GetGeometry() const;
    void SetGeometry( const std::string& name );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateStructure();
    void CreatePolygonGeometry( const TerrainObject& shape );
    void CreateLineGeometry( const TerrainObject& shape );
    void CreateGeometry( const TerrainObject& shape );
    void MbrSpatialIndex();
    //@}

private:
    //! @name Member data
    //@{
    sqlite3* db_;
    std::string name_;
    std::string geometry_;
    gaiaGeomCollPtr geo_;
    char* err_msg_;
    //@}
};

} //! namespace geostore

#endif // __GeoTable_h_
