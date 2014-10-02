// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dialogs_h_
#define __Dialogs_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class ColorEditor_ABC;
}

namespace actions
{
    class ActionsModel;
}

namespace tools
{
    class ExerciseConfig;
}

class StaticModel;
class Publisher_ABC;
class CommandHandler;

// =============================================================================
/** @class  Dialogs
    @brief  Dialogs
*/
// Created: AGE 2006-04-20
// =============================================================================
class Dialogs : public QObject
{
public:
    //! @name Constructors/Destructor
    //@{
             Dialogs( QWidget* parent,
                      kernel::Controllers& controllers,
                      const StaticModel& staticModel,
                      Publisher_ABC& publisher,
                      actions::ActionsModel& actionsModel,
                      const kernel::Time_ABC& simulation,
                      const kernel::Profile_ABC& profile,
                      CommandHandler& handler,
                      const tools::ExerciseConfig& config,
                      gui::ColorStrategy_ABC& colorStrategy,
                      gui::ColorEditor_ABC& colorEditor );
    virtual ~Dialogs();
    //@}
};

#endif // __Dialogs_h_
