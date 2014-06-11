// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __SmoothPositions_h_
#define __SmoothPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

class Simulation;

// =============================================================================
/** @class  SmoothPositions
    @brief  SmoothPositions
*/
// Created: SLI 2014-06-05
// =============================================================================
class SmoothPositions : public kernel::Positions
                      , public kernel::OptionsObserver_ABC
                      , public tools::Observer_ABC
                      , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SmoothPositions( kernel::Controllers& controllers, const Simulation& simulation, const kernel::Positions& positions );
    virtual ~SmoothPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual void Compute();
    //@}

private:
    //! @name Options
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

    //! @name Helpers
    //@{
    void InitializePositions( const geometry::Point2f& position );
    void ComputeCurrentPosition();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    const kernel::Positions& positions_;
    geometry::Point2f currentPosition_;
    geometry::Point2f futurePosition_;
    geometry::Vector2f step_;
    int remainingSteps_;
    int lastTick_;
    float refreshRate_;
    bool smoothEnabled_;
    //@}
};

#endif // __SmoothPositions_h_
