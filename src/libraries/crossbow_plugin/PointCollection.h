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

namespace Common
{
    class MsgLocation;
    class MsgCoordLatLongList;
}

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
    virtual void Serialize( Common::MsgLocation& message ) const;
    //@}

    //! @name
    //@{
    virtual OGRGeometry* Extract( OGRSpatialReference* spatialReference ) const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             PointCollection();
    explicit PointCollection( const Common::MsgCoordLatLongList& message );
    explicit PointCollection( const OGRLineString& geometry );
    //@}

    //! @name
    //@{
    virtual OGRLineString* Extract( OGRLineString* points, OGRSpatialReference* spatialReference ) const;
    virtual void Serialize( std::ostream& geometry ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PointCollection( const PointCollection& );            //!< Copy constructor
    PointCollection& operator=( const PointCollection& ); //!< Assignement operator
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
