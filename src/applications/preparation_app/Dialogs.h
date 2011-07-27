// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Dialogs_h_
#define __Dialogs_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class ColorEditor_ABC;
    class EntitySymbols;
}

namespace tools
{
    class ExerciseConfig;
}

class StaticModel;

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
             Dialogs( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, const kernel::Profile_ABC& profile,
                      gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor, const gui::EntitySymbols& symbols,
                      const tools::ExerciseConfig& config );
    virtual ~Dialogs();
    //@}
};

#endif // __Dialogs_h_
