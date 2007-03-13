// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationMissionInterface_h_
#define __PopulationMissionInterface_h_

#include "MissionInterface_ABC.h"

namespace kernel
{
    class Mission;
}

class ASN_MsgPopulationOrder;
class Publisher_ABC;
class MissionInterfaceFactory;
class MissionInterfaceBuilder;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class PopulationMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationMissionInterface( QWidget* parent, kernel::Entity_ABC& entity, const kernel::Mission& mission, kernel::ActionController& controller
                                       , Publisher_ABC& publisher, MissionInterfaceFactory& factory, MissionInterfaceBuilder& builder );
    virtual ~PopulationMissionInterface();
    //@}
   
private:
    //! @name Copy/Assignment
    //@{
    PopulationMissionInterface( const PopulationMissionInterface& );
    PopulationMissionInterface& operator=( const PopulationMissionInterface& );
    //@}

    //! @name Helpers
    //@{
    virtual void Publish();
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC&          publisher_;
    ASN_MsgPopulationOrder* order_;
    //@}
};

#endif // __PopulationMissionInterface_h_
