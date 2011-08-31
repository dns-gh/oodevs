//Added by qt3to4:
#include <Qt3Support/q3button.h>
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoButtonsWidget_h_
#define __InfoButtonsWidget_h_

#include <boost/noncopyable.hpp>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

class StaticModel;

// =============================================================================
/** @class  InfoButtonsWidget
    @brief  InfoButtonsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoButtonsWidget : public Q3GroupBox
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const StaticModel& staticModel,
                                actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile );
    virtual ~InfoButtonsWidget();
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Dialog >
    void AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    void AddButton( QDialog* dialog, const QPixmap& pixmap, const QString& tooltips, const char* toggleSlot, const char* toggleSignal );
    //@}
};

#endif // __InfoButtonsWidget_h_
