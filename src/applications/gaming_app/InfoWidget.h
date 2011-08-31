// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoWidget_h_
#define __InfoWidget_h_

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
    class EntitySymbols;
    class ItemFactory_ABC;
}

class StaticModel;

// =============================================================================
/** @class  InfoWidget
    @brief  InfoWidget
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoWidget : public Q3HBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons,
                         gui::ItemFactory_ABC& itemFactory, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation );
    virtual ~InfoWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoWidget( const InfoWidget& );            //!< Copy constructor
    InfoWidget& operator=( const InfoWidget& ); //!< Assignment operator
    //@}
};

#endif // __InfoWidget_h_
