// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostPositions_h_
#define __GhostPositions_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Ghost_ABC;
    class Controller;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  GhostPositions
    @brief  GhostPositions
*/
// Created: ABR 2011-10-19
// =============================================================================
class GhostPositions : public kernel::Moveable_ABC
                     , public kernel::Serializable_ABC
                     , public gui::Drawable_ABC
                     , public boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostPositions( const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller,
                             const geometry::Point2f& position, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             GhostPositions( xml::xistream& xis, const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter,
                             kernel::Controller& controller, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~GhostPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual bool CanAggregate() const;
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& position );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Ghost_ABC& ghost_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    const kernel::Entity_ABC& entity_;
    kernel::Moveable_ABC* moveable_;
    geometry::Point2f position_;
    float height_;
    //@}
};

#endif // __GhostPositions_h_
