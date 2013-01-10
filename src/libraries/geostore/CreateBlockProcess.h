// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateBlockProcess_h_
#define __CreateBlockProcess_h_

class PointProjector_ABC;

namespace geostore
{
    class GeometryFactory;
    class SpatialIndexer;

// =============================================================================
/** @class  CreateBlockProcess
    @brief  CreateBlockProcess
*/
// Created: AME 2010-08-05
// =============================================================================
class CreateBlockProcess : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CreateBlockProcess();
    virtual ~CreateBlockProcess();
    //@}

    //! @name Operations
    //@{
    bool CanCreateBlock( const SpatialIndexer& indexer, const geometry::Polygon2f& footprint, PointProjector_ABC& projector, unsigned int idToExclude = 0 );
    //@}

private:
    //! @name Member data
    //@{
    GeometryFactory* geometryFactory_;
    //@}
};

}

#endif // __CreateBlockProcess_h_
