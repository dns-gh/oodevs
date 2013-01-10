// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __GeometryFactory_h_
#define __GeometryFactory_h_

#include <boost/noncopyable.hpp>

class PointProjector_ABC;

namespace geostore
{
// =============================================================================
/** @class  GeometryFactory
    @brief  GeometryFactory
*/
// Created: AME 2010-07-30
// =============================================================================
class GeometryFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GeometryFactory();
    virtual ~GeometryFactory();
    //@}

    //! @name Operations
    //@{
    static gaiaGeomCollPtr Validate( gaiaGeomCollPtr geom );
    gaiaGeomCollPtr InitGeometryCollection();
    gaiaGeomCollPtr CreatePolygonGeometry( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector );
    void AddPolygonGeometryToCollection( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaGeomCollPtr& geomColl );
    //@}

private:
    //! @name Helpers
    //@{
    void FillPolygon( const geometry::Polygon2f& footPrint, PointProjector_ABC& projector, gaiaPolygonPtr& polyg ) const;
    //@}
};

}

#endif // __GeometryFactory_h_
