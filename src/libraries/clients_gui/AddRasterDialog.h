// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __AddRasterDialog_h_
#define __AddRasterDialog_h_

#include "ModalDialog.h"

namespace gui
{
    class SpinBoxAndSlider;
    template< typename T > class RichWidget;
    class RichPushButton;
    class RichLineEdit;

// =============================================================================
/** @class  AddRasterDialog
    @brief  AddRasterDialog
*/
// Created: ABR 2012-06-12
// =============================================================================
class AddRasterDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AddRasterDialog( QWidget* parent );
    virtual ~AddRasterDialog();
    //@}

    //! @name Operations
    //@{
    const QString GetName() const;
    const QString GetFiles() const;
    int GetPixelSize() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged();
    void OnBrowse();
    //@}

private:
    //! @name Member data
    //@{
    int               pixelSize_;
    RichWidget< QTabWidget >*    tabWidget_;
    SpinBoxAndSlider* pixelSizeEditor_;
    RichLineEdit*        nameEditor_;
    RichLineEdit*        fileEditor_;
    QLabel*           warningLabel_;
    RichPushButton*   okButton_;
    //@}
};

} //! namespace gui

#endif // __AddRasterDialog_h_
