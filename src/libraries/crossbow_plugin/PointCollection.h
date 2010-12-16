// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PointCollection_h_
#define __PointCollection_h_

#include "Shape_ABC.h"

namespace sword
{
    class CoordLatLongList;
}

class OGRLineString;

namespace plugins
{
namespace crossbow
{
    class Point;

// =============================================================================
/** @class  PointCollection
*/
// Created: JCR 2008-04-25
// =============================================================================
class PointCollection : public Shape_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~PointCollection();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( kernel::Location_ABC& location, const kernel::CoordinateConverter_ABC& converter ) const;
    //@}

    //! @name
    //@{
    virtual void Serialize( OGRFeature& feature, OGRSpatialReference* spatialReference ) const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             PointCollection();
    explicit PointCollection( const sword::CoordLatLongList& message );
    explicit PointCollection( const OGRLineString& geometry );
    //@}

    //! @name
    //@{
    virtual void Serialize( OGRLineString& points, OGRSpatialReference* spatialReference ) const;
    virtual void Serialize( std::ostream& geometry ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PointCollection( const PointCollection& );            //!< Copy constructor
    PointCollection& operator=( const PointCollection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< Point >       T_Points;
    typedef T_Points::const_iterator CIT_Points;
    //@}

private:
    //! @name Member data
    //@{
    T_Points points_;
    //@}
};

}
}

#endif // __PointCollection_h_
