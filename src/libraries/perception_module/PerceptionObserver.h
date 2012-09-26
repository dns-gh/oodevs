// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_OBSERVER_H
#define SWORD_PERCEPTION_PERCEPTION_OBSERVER_H

#include "PerceptionObserver_ABC.h"
#include "wrapper/Effect.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  PerceptionObserver
    @brief  Perception observer
*/
// Created: SLI 2012-06-19
// =============================================================================
class PerceptionObserver : public PerceptionObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PerceptionObserver( const wrapper::View& perceiver );
    virtual ~PerceptionObserver();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUrbanPerception( const wrapper::View& urbanBlock, const PerceptionLevel& level );
    virtual void NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level );
    virtual void NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level, bool perceptionRecorded );
    virtual void NotifyObjectPerception( const wrapper::View& object, const PerceptionLevel& level );
    virtual void NotifyFlowPerception( const wrapper::View& flow, const PerceptionLevel& level, const std::vector< MT_Vector2D >& shape );
    virtual void NotifyConcentrationPerception( const wrapper::View& concentration, const PerceptionLevel& level );
    //@}

private:
    //! @name Member data
    //@{
    const bool recordModeEnabled_;
    wrapper::Effect agentNotifications_;
    wrapper::Effect objectNotifications_;
    wrapper::Effect urbanBlockNotifications_;
    wrapper::Effect flowNotifications_;
    wrapper::Effect concentrationNotifications_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_OBSERVER_H
