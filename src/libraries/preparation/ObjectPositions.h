// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public kernel::Positions
                      , public kernel::Serializable_ABC
                      , public kernel::Drawable_ABC
                      , public kernel::LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ObjectPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ObjectPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis );
    void ReadPoint( xml::xistream& xis );
    void Update();
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Location_ABC* location_;
    geometry::Rectangle2f boundingBox_;

    T_PointVector points_;
    //@}

};

#endif // __ObjectPositions_h_
