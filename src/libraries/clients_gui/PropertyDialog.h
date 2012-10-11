// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyDialog_h
#define gui_PropertyDialog_h

namespace gui
{
// =============================================================================
/** @class  PropertyDialog
    @brief  Property dialog
*/
// Created: LGY 2012-08-16
// =============================================================================
class PropertyDialog : public QDialog
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertyDialog( QWidget* parent );
    virtual ~PropertyDialog();
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* event );
    //@}
};

}

#endif // gui_PropertyDialog_h
