// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationBase_h_
#define __LocationBase_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class Viewport_ABC;
    class GlTools_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  LocationBase
    @brief  LocationBase
*/
// Created: SBO 2007-04-25
// =============================================================================
class LocationBase : public kernel::LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationBase( const kernel::CoordinateConverter_ABC& converter, const ASN1T_Location& asn );
             LocationBase( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             LocationBase( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~LocationBase();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    geometry::Point2f GetPosition() const;
    void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    void CommitTo( std::string& content ) const;
    void CommitTo( ASN1T_Location& asn ) const;
    void Clean( ASN1T_Location& asn ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    void PushFront( const geometry::Point2f& point );
    void PushBack( const geometry::Point2f& point );
    bool IsValid() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationBase( const LocationBase& );            //!< Copy constructor
    LocationBase& operator=( const LocationBase& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ReadPoint( xml::xistream& xis );
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    virtual void VisitPath   ( const geometry::Point2f& first, const T_PointVector& points );
    void Draw( const kernel::GlTools_ABC& tools ) const;
    bool CheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    E_LocationType type_;
    T_PointVector points_;
    geometry::Rectangle2f boundingBox_;
    bool valid_;
    //@}
};

    }
}

#endif // __LocationBase_h_
