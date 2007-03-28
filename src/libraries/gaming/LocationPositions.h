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
#include "game_asn/Asn.h"

namespace kernel
{
    class CoordinateConverter_ABC;
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
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;

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
    //@}

protected:
    //! @name Helpers
    //@{
    void Update( const ASN1T_Localisation& localisation );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector points_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f center_;
    //@}
};

#endif // __LocationPositions_h_
