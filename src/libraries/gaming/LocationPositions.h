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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Positions.h"

namespace sword
{
    class Location;
}

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
                        , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationPositions( const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual bool CanAggregate() const;
    const kernel::Location_ABC* GetLocation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationPositions( const LocationPositions& );            //!< Copy constructor
    LocationPositions& operator=( const LocationPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Draw( const gui::GLView_ABC& tools ) const;
    void AddLocation( const sword::Location& message );
    void AddPoint( const geometry::Point2f& point );
    //@}

protected:
    //! @name Helpers
    //@{
    void Update( const sword::Location& message );
    void Update( const sword::Location& message, const geometry::Point2f& startPoint );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector points_;
    std::unique_ptr< kernel::Location_ABC > location_;

protected:
    geometry::Rectangle2f boundingBox_;
    //@}
};

#endif // __LocationPositions_h_
