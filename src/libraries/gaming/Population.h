// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_


#include "clients_kernel/Types.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Displayable_ABC.h"
#include "Simulation.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class PopulationType;
    class CoordinateConverter_ABC;
    class Displayer_ABC;
}

class PopulationPart_ABC;
class PopulationConcentration;
class PopulationFlow;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public kernel::EntityImplementation< kernel::Population_ABC >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowCreation > // $$$$ AGE 2006-03-13: d�gager dans des extensions ?
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowDestruction >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationDestruction >
                 , public kernel::Drawable_ABC
                 , public kernel::Positions
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const MsgsSimToClient::MsgCrowdCreation& message, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter,
                         const tools::Resolver_ABC< kernel::PopulationType >& typeResolver );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    unsigned int GetLivingHumans() const;
    unsigned int GetDeadHumans() const;

    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

    //! @name Accessors
    //@{
    const kernel::PopulationType& GetType() const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

    //! @name Helpers
    //@{
    unsigned int ComputeLivingHumans() const;
    unsigned int ComputeDeadHumans() const;

    void DoUpdate( const MsgsSimToClient::MsgCrowdFlowCreation&             message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdFlowUpdate&               message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdFlowDestruction&          message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdUpdate&                   message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationCreation&    message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationUpdate&      message );
    void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationDestruction& message );

    void ComputeCenter();

    virtual void NotifyUpdated( const Simulation::sEndTick& tick );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                   controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::PopulationType&          type_;

    geometry::Rectangle2f                  boundingBox_;
    geometry::Point2f                      center_;
    kernel::OptionalValue< int >           nDomination_;
    //@}

private:
    static unsigned long nMaxId_;
    std::set< kernel::Displayer_ABC* > displayers_;
};

#endif // __Population_h_
