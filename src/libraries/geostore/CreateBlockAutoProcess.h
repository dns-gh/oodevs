// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateBlockAutoProcess_h_
#define __CreateBlockAutoProcess_h_

class PointProjector_ABC;
class UrbanModel;

namespace kernel
{
    class UrbanObject_ABC;
}

namespace geostore
{
    class Database;
    class GeometryFactory;
    class SpatialRequestStatus;

// =============================================================================
/** @class  CreateBlockAutoProcess
    @brief  CreateBlockAutoProcess
*/
// Created: AME 2010-08-02
// =============================================================================
class CreateBlockAutoProcess : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CreateBlockAutoProcess( const Database& database );
    virtual ~CreateBlockAutoProcess();
    //@}

    //! @name Operations
    //@{
    void Run( const geometry::Polygon2f& footprint, UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateUrbanModel( UrbanModel& model, kernel::UrbanObject_ABC& parent, PointProjector_ABC& projector, const geometry::Polygon2f& footprint );
    gaiaGeomCollPtr GetUrbanBlockInArea( const UrbanModel& model, PointProjector_ABC& projector, const geometry::Polygon2f& footprint );
    void UpdateBuildingTable( gaiaGeomCollPtr buildings );
    void ClippingUrbanAreaWithTerrainComponent( gaiaGeomCollPtr buffers, gaiaGeomCollPtr areas, gaiaGeomCollPtr urbans );
    void PrepareTerrainComponents( gaiaGeomCollPtr selection, gaiaGeomCollPtr& areas, gaiaGeomCollPtr& buffers );
    //@}

private:
    //! @name Member data
    //@{
    const Database&                     database_;
    std::auto_ptr< GeometryFactory >    geometryFactory_;
    gaiaGeomCollPtr                     blocks_;
    //@}
};

}

#endif // __CreateBlockAutoProcess_h_
