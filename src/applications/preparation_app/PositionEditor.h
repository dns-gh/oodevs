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

#include "clients_gui/ModalDialog.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ValueEditor.h"

namespace kernel
{
    class Controllers;
    class Moveable_ABC;
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
class PositionEditor : public ModalDialog
                     , public kernel::ValueEditor< kernel::Moveable_ABC* >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PositionEditor( QDialog*& self, QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
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
    //! @name Copy/Assignment
    //@{
    PositionEditor( const PositionEditor& );            //!< Copy constructor
    PositionEditor& operator=( const PositionEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Point2f lastValid_;
    kernel::Moveable_ABC** value_;
    gui::LocationEditorBox* locBox_;
    QDialog*& self_;
    //@}
};

#endif // __PositionEditor_h_
