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

class InfoDialog;

// =============================================================================
/** @class  InfoButtonsWidget
    @brief  InfoButtonsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoButtonsWidget : public QGroupBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoButtonsWidget( QWidget* widget, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoButtonsWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggle( bool state );
    void OnClose();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoButtonsWidget( const InfoButtonsWidget& );            //!< Copy constructor
    InfoButtonsWidget& operator=( const InfoButtonsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddButton( const QPixmap& pixmap, InfoDialog* dialog );
    //@}

    //! @name Types
    //@{
    typedef std::map< QButton*, QWidget* > T_ButtonWidgets;
    typedef T_ButtonWidgets::iterator     IT_ButtonWidgets;
    //@}

private:
    //! @name Member data
    //@{
    T_ButtonWidgets widgets_;
    //@}
};

#endif // __InfoButtonsWidget_h_
