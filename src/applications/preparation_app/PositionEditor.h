// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PositionEditor_h_
#define __PositionEditor_h_

#include "clients_gui/PropertyDialog.h"
#include "clients_gui/ValueEditor.h"

namespace kernel
{
    class Controllers;
    class Moveable_ABC;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class LocationEditorBox;
}

// =============================================================================
/** @class  PositionEditor
    @brief  PositionEditor
*/
// Created: AME 2010-03-08
// =============================================================================
class PositionEditor : public gui::PropertyDialog
                     , public gui::ValueEditor< kernel::Moveable_ABC* >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PositionEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PositionEditor();
    //@}

    //! @name Operations
    //@{
    void SetValue( kernel::Moveable_ABC*& value );
    virtual kernel::Moveable_ABC* GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::Moveable_ABC** value_;
    gui::LocationEditorBox* locBox_;
    //@}
};

#endif // __PositionEditor_h_
