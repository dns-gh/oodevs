// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoDialog_h_
#define __InfoDialog_h_

#include "clients_gui/PanelStack_ABC.h"

// =============================================================================
/** @class  InfoDialog
    @brief  InfoDialog
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoDialog : public QDialog
                 , public gui::PanelStack_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoDialog( QWidget* parent, const QString& title );
    virtual ~InfoDialog();
    //@}

    //! @name Operations
    //@{
    virtual void Add( QWidget* widget );
    virtual void Remove( QWidget* widget );
    virtual QSize sizeHint() const;
    QWidget* RootWidget();
    virtual void hideEvent( QHideEvent* );
    //@}

signals:
    //! @name Signal
    //@{
    void Closed();
    //@}

private:
    //! @name Member data
    //@{
    QVBox* box_;
    //@}
};

#endif // __InfoDialog_h_
