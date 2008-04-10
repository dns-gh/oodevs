// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPanel_h_
#define __ObjectPanel_h_

#include "clients_gui/ObjectPanel.h"

namespace gui
{
    class SpinBoxDisplayer;
    class CheckBoxDisplayer;
}

class Publisher_ABC;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: AGE 2006-09-08
// =============================================================================
class ObjectPanel : public gui::ObjectPanel
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher );
    virtual ~ObjectPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnApply();
    void OnCancel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPanel( const ObjectPanel& );            //!< Copy constructor
    ObjectPanel& operator=( const ObjectPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    gui::SpinBoxDisplayer* construction_;
    gui::SpinBoxDisplayer* valorisation_;     
    gui::SpinBoxDisplayer* contournement_;
    //@}
}; 

#endif // __ObjectPanel_h_
