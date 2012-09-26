// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_OBSERVER_ABC_H
#define SWORD_PERCEPTION_PERCEPTION_OBSERVER_ABC_H

#include <boost/noncopyable.hpp>
#include <vector>

class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;

// =============================================================================
/** @class  PerceptionObserver_ABC
    @brief  Perception observer definition
*/
// Created: SLI 2012-04-12
// =============================================================================
class PerceptionObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionObserver_ABC() {}
    virtual ~PerceptionObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUrbanPerception( const wrapper::View& urbanBlock, const PerceptionLevel& level ) = 0;
    virtual void NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level ) = 0;
    virtual void NotifyAgentPerception( const wrapper::View& agent, const PerceptionLevel& level, bool perceptionRecorded ) = 0;
    virtual void NotifyObjectPerception( const wrapper::View& object, const PerceptionLevel& level ) = 0;
    virtual void NotifyFlowPerception( const wrapper::View& flow, const PerceptionLevel& level, const std::vector< MT_Vector2D >& shape ) = 0;
    virtual void NotifyConcentrationPerception( const wrapper::View& concentration, const PerceptionLevel& level ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_OBSERVER_ABC_H
