// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Location_h_
#define __Location_h_

#include "game_asn/Asn.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

// =============================================================================
/** @class  Location
    @brief  Location
    // $$$$ SBO 2007-04-25: kernel::Positions maybe
*/
// Created: SBO 2007-04-25
// =============================================================================
class Location : public kernel::LocationVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Location( const kernel::CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn );
             Location( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
    virtual ~Location();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    geometry::Point2f GetPosition() const;
    void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    void PushFront( const geometry::Point2f& point );
    void PushBack( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Location( const Location& );            //!< Copy constructor
    Location& operator=( const Location& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    void Draw( const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    ASN1T_EnumTypeLocalisation type_;
    T_PointVector points_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

#endif // __Location_h_
