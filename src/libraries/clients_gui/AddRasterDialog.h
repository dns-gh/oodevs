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
#include <boost/noncopyable.hpp>

namespace gui
{
    class SpinBoxAndSlider;

// =============================================================================
/** @class  AddRasterDialog
    @brief  AddRasterDialog
*/
// Created: ABR 2012-06-12
// =============================================================================
class AddRasterDialog : public ModalDialog
                      , private boost::noncopyable
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
    QTabWidget*       tabWidget_;
    SpinBoxAndSlider* pixelSizeEditor_;
    QLineEdit*        nameEditor_;
    QLineEdit*        fileEditor_;
    QLabel*           warningLabel_;
    QPushButton*      okButton_;
    //@}
};

} //! namespace gui

#endif // __AddRasterDialog_h_
