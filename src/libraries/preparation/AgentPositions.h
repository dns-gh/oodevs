// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentPositions_h_
#define __AgentPositions_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Agent_ABC;
    class PropertiesDictionary;
    class Controller;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AgentPositions
    @brief  AgentPositions. Nothing sexual.
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentPositions : public kernel::Moveable_ABC
                     , public kernel::Aggregatable_ABC
                     , public kernel::Serializable_ABC
                     , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentPositions( kernel::Agent_ABC& agent, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, const geometry::Point2f& position, kernel::PropertiesDictionary& dico );
             AgentPositions( xml::xistream& xis, kernel::Agent_ABC& agent, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller, kernel::PropertiesDictionary& dico );
    virtual ~AgentPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& position );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentPositions( const AgentPositions& );            //!< Copy constructor
    AgentPositions& operator=( const AgentPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Agent_ABC& agent_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Controller& controller_;
    kernel::Moveable_ABC* moveable_;
    geometry::Point2f position_;
    float height_;
    bool aggregated_;
    //@}
};

#endif // __AgentPositions_h_
