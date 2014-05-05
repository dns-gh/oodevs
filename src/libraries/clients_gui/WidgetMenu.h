// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_WidgetMenu_h
#define __gui_WidgetMenu_h

namespace gui
{

// =============================================================================
/** @class  WidgetMenu
    @brief  WidgetMenu
*/
// Created: ABR 2014-04-29
// =============================================================================
class WidgetMenu : public QMenu
{
public:
    //! @name Constructors/Destructor
    //@{
             WidgetMenu( QWidget* parent );
    virtual ~WidgetMenu();
    //@}

    //! @name Operations
    //@{
    void AddWidget( QWidget* widget );
    //@}

private:
    //! @name Member data
    //@{
    QVBoxLayout* layout_;
    //@}
};

} //! namespace gui

#endif // __gui_WidgetMenu_h
