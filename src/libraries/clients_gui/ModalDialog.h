// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModalDialog_h_
#define __ModalDialog_h_

// =============================================================================
/** @class  ModalDialog
    @brief  ModalDialog
*/
// Created: JSR 2012-03-20
// =============================================================================
class ModalDialog : public QDialog
{
public:
    //! @name Constructors/Destructor
    //@{
             ModalDialog( QWidget *parent, const char *name, bool modal = true, Qt::WindowFlags f = 0);
    virtual ~ModalDialog();
    //@}
};

#endif // __ModalDialog_h_
