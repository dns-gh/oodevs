// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_h_
#define __MissionInterface_h_

#include "MissionInterface_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class OrderType;
    class Time_ABC;
}


namespace tools
{
    class ExerciseConfig;
    class Path;
}

namespace actions
{
    class Action_ABC;
    class ActionsModel;

    namespace gui
    {
        class Param_ABC;
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  MissionInterface
    @brief  MissionInterface
*/
// Created: APE 2004-04-20
// =============================================================================
template< typename T >
class MissionInterface : public MissionInterface_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            MissionInterface( QWidget* parent, const QString& name, kernel::Controllers& controllers,
                              const tools::ExerciseConfig& config, const std::string& missionSheetPhysicalTag, actions::ActionsModel& model )
            : MissionInterface_ABC( parent, name, controllers, config, missionSheetPhysicalTag )
            , model_( model )
            {
                // NOTHING
            }
    virtual ~MissionInterface()
            {
                // NOTHING
            }
    //@}

    //! @name Operations
    //@{
    void Publish()
    {
        if( !order_ )
            return;
        Action_ABC* action = model_.CreateAction( *entity_, static_cast< const T& >( *order_ ) );
        CommitTo( *action );
        if( planned_ )
            emit PlannedMission( *action );
        else
            model_.Publish( *action, 0 );
    }
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& model_;
    //@}

};

    }
}

#endif // __MissionInterface_h_
