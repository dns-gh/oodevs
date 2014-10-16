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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
}

namespace gui
{
    class PropertiesDictionary;
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
class PopulationPositions : public kernel::Moveable_ABC
                          , public kernel::Serializable_ABC
                          , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationPositions( Population& owner, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                                  const geometry::Point2f& position, gui::PropertiesDictionary& dictionary );
             PopulationPositions( xml::xistream& xis, Population& owner, kernel::Controller& controller,
                                  const kernel::CoordinateConverter_ABC& converter, gui::PropertiesDictionary& dictionary );
    virtual ~PopulationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual void Move( const geometry::Point2f& position );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationPositions( const PopulationPositions& );            //!< Copy constructor
    PopulationPositions& operator=( const PopulationPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    static geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    void UpdatePosition();
    void CreateDictionary( gui::PropertiesDictionary& dictionary );

    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    Population& owner_;
    kernel::Controller& controller_;
    kernel::Moveable_ABC* moveable_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f center_;
    unsigned long livingHumans_;
    float radius_;
    //@}
};

#endif // __PopulationPositions_h_
