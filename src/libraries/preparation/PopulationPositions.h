// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPositions_h_
#define __PopulationPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

class Population;

// =============================================================================
/** @class  PopulationPositions
    @brief  PopulationPositions
*/
// Created: SBO 2006-11-08
// =============================================================================
class PopulationPositions : public kernel::Positions
                          , public kernel::Drawable_ABC
                          , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPositions( const Population& owner, const kernel::CoordinateConverter_ABC& converter, const geometry::Point2f& position );
             PopulationPositions( xml::xistream& xis, const Population& owner, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    //@}

    //! @name Modifiers
    //@{
    void Set( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationPositions( const PopulationPositions& );            //!< Copy constructor
    PopulationPositions& operator=( const PopulationPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    static geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    void UpdatePosition();

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const Population&                      owner_;
    geometry::Rectangle2f                  boundingBox_;
    geometry::Point2f                      center_;
    unsigned long                          livingHumans_;
    float                                  radius_;
    //@}
};

#endif // __PopulationPositions_h_
