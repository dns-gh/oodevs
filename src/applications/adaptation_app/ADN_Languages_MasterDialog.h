// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Languages_MasterDialog_h_
#define __ADN_Languages_MasterDialog_h_

#include "clients_gui/RichWidget.h"

class ADN_Languages_Data;

// =============================================================================
/** @class  ADN_Languages_MasterDialog
    @brief  ADN_Languages_MasterDialog
*/
// Created: ABR 2013-10-03
// =============================================================================
class ADN_Languages_MasterDialog : public gui::RichWidget< QDialog >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Languages_MasterDialog( ADN_Languages_Data& data );
    virtual ~ADN_Languages_MasterDialog();
    //@}

signals:
    //! @name Signals
    //@{
    void MasterChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void showEvent( QShowEvent* pEvent );
    virtual void accept();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Languages_Data& data_;
    QStandardItemModel model_;
    QComboBox* combo_;
    //@}
};

#endif // __ADN_Languages_MasterDialog_h_
