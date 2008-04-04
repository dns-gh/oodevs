// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Line_h_
#define __Line_h_

#include "game_asn/Simulation.h"
#include "Shape_ABC.h"
#include "ESRI.h"

namespace crossbow
{
    class Point;

// =============================================================================
/** @class  Line
    @brief  Line
*/
// Created: SBO 2007-08-30
// =============================================================================
class Line : public Shape_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Line();
    explicit Line( const ASN1T_CoordLatLongList& asn );
    explicit Line( IGeometryPtr geometry );
    virtual ~Line();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ShapeVisitor_ABC& visitor ) const;
    void UpdateGeometry( IGeometryPtr geometry, ISpatialReferencePtr spatialReference ) const;
    void Serialize( ASN1T_Location& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Line( const Line& );            //!< Copy constructor
    Line& operator=( const Line& ); //!< Assignment operator
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

#endif // __Line_h_
