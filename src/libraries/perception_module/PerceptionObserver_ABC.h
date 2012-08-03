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

class UrbanObjectWrapper;
class MIL_Object_ABC;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
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
    virtual void NotifyPerceptionUrban( const UrbanObjectWrapper* urbanBlock, const PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( const wrapper::View& agent, const PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( const wrapper::View& agent, const PerceptionLevel& level, bool perceptionRecorded ) = 0;
    virtual void NotifyPerception( const MIL_Object_ABC* object, const PerceptionLevel& level ) = 0;
    virtual void NotifyPerception( const MIL_PopulationFlow* flow, const PerceptionLevel& level, const std::vector< MT_Vector2D >& shape ) = 0;
    virtual void NotifyPerception( const MIL_PopulationConcentration* concentration, const PerceptionLevel& level ) = 0;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_OBSERVER_ABC_H
