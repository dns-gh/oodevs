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

#include "game_asn/Simulation.h"
#include "Shape_ABC.h"
#include "ESRI.h"

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
    virtual void Accept( ShapeVisitor_ABC& visitor ) const;
    virtual void Serialize( ASN1T_Location& asn ) const;
    //@}

    //! @name 
    //@{
    virtual void UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;    
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             PointCollection();
    explicit PointCollection( const ASN1T_CoordLatLongList& asn );
    explicit PointCollection( IGeometryPtr geometry );    
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
