// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentPositions_h_
#define __AgentPositions_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"

namespace sword
{
    class UnitAttributes;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class Agent_ABC;
}

// =============================================================================
/** @class  AgentPositions
    @brief  AgentPositions. Nothing sexual.
*/
// Created: AGE 2006-03-16
// =============================================================================
class AgentPositions : public kernel::Positions
                     , public kernel::Updatable_ABC< sword::UnitAttributes >
                     , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentPositions( kernel::Controller& controller, const kernel::Agent_ABC& agent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AgentPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentPositions( const AgentPositions& );            //!< Copy constructor
    AgentPositions& operator=( const AgentPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

private:
    friend class AggregatedPositions; // $$$$ AGE 2006-10-06:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Agent_ABC& agent_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f position_;
    float height_;
    bool dead_;
    //@}
};

#endif // __AgentPositions_h_
