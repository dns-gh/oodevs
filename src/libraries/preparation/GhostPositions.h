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

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Ghost_ABC;
    class PropertiesDictionary;
    class Controller;
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
                     , public kernel::Drawable_ABC
                     , public kernel::Aggregatable_ABC
                     , public kernel::Serializable_ABC
                     , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostPositions( const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, const geometry::Point2f& position, kernel::PropertiesDictionary& dico );
             GhostPositions( xml::xistream& xis, const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, kernel::PropertiesDictionary& dico );
    virtual ~GhostPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& position );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateDictionary( kernel::PropertiesDictionary& dico );
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Ghost_ABC& ghost_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    kernel::Moveable_ABC* moveable_;
    geometry::Point2f position_;
    float height_;
    bool aggregated_;
    //@}
};

#endif // __GhostPositions_h_
