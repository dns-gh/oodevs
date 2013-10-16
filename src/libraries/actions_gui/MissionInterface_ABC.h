// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#include "ENT/ENT_Tr.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class OrderType;
    class Entity_ABC;
}

namespace timeline
{
    struct Event;
}

namespace actions
{
    class ActionsModel;
    class Action_ABC;
namespace gui
{
    class InterfaceBuilder_ABC;
// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: LGY 2013-10-11
// =============================================================================
class MissionInterface_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC() {}
    virtual ~MissionInterface_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( InterfaceBuilder_ABC& builder, const kernel::OrderType& orderType, E_MissionType type ) = 0;
    virtual void Purge() = 0;
    virtual void SetEntity( const kernel::Entity_ABC* entity ) = 0;
    virtual void SetPlanned( bool planned ) = 0;
    virtual void PublishFragOrder( actions::ActionsModel& model, timeline::Event* event = 0 ) const = 0;
    virtual void PublishMissionOrder( actions::ActionsModel& model, timeline::Event* event = 0 ) const = 0;
    virtual void FillFrom( const actions::Action_ABC& action ) = 0;
    //@}

};

}
}

#endif // __MissionInterface_ABC_h_
