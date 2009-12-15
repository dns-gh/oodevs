// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocationPositions_h_
#define __LocationPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

// =============================================================================
/** @class  LocationPositions
    @brief  LocationPositions
*/
// Created: AGE 2006-05-18
// =============================================================================
class LocationPositions : public kernel::Positions
                        , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationPositions( const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LocationPositions( const LocationPositions& );            //!< Copy constructor
    LocationPositions& operator=( const LocationPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Draw( const kernel::GlTools_ABC& tools ) const;
    void AddLocation( const ASN1T_Location& asn );
    void AddPoint( const geometry::Point2f& point );
    //@}

protected:
    //! @name Helpers
    //@{
    const kernel::Location_ABC* GetLocation() const;
    void Update( const ASN1T_Location& asn );
    void Update( const ASN1T_Location& asn, const geometry::Point2f& startPoint );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector points_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f center_;
    std::auto_ptr< kernel::Location_ABC > location_;
    //@}
};

#endif // __LocationPositions_h_
