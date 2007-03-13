// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitMissionInterface_h_
#define __UnitMissionInterface_h_

#include "MissionInterface_ABC.h"

namespace kernel
{
    class Mission;
}

class ASN_MsgPionOrder;
class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class UnitMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::Mission& mission, kernel::ActionController& controller
                                 , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder );
    virtual ~UnitMissionInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UnitMissionInterface( const UnitMissionInterface& );
    UnitMissionInterface& operator=( const UnitMissionInterface& );
    //@}

    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC&    publisher_;
    ASN_MsgPionOrder* order_;
    //@}
};

#endif // __UnitMissionInterface_h_
