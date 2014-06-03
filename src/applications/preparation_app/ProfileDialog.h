// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileDialog_h_
#define __ProfileDialog_h_

#include "clients_gui/ModalDialog.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class EntitySymbols;
}

class UserProfileList;
class UserProfileWidget;
class Model;

// =============================================================================
/** @class  ProfileDialog
    @brief  ProfileDialog
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfileDialog( QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons, Model& model );
    virtual ~ProfileDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    //@}

private:
    //! @name Member data
    //@{
    UserProfileList* list_;    //@}
};

#endif // __ProfileDialog_h_
