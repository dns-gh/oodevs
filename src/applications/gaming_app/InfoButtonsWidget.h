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

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoButtonsWidget
    @brief  InfoButtonsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoButtonsWidget : public Q3GroupBox
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoButtonsWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoButtonsWidget( const InfoButtonsWidget& );            //!< Copy constructor
    InfoButtonsWidget& operator=( const InfoButtonsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename Dialog >
    void AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    //@}

    //! @name Types
    //@{
    typedef std::map< QPushButton*, QWidget* > T_ButtonWidgets;
    typedef T_ButtonWidgets::iterator     IT_ButtonWidgets;
    //@}

private:
    //! @name Member data
    //@{
    T_ButtonWidgets widgets_;
    //@}
};

#endif // __InfoButtonsWidget_h_
