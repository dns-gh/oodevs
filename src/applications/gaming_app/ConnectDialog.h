//****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************

#ifndef __ConnectDialog_h_
#define __ConnectDialog_h_

#include "clients_gui/Types.h"

namespace kernel
{
    class Logger_ABC;
}

class QComboBox;
class QSpinBox;
class QCheckBox;
class QLabel;
class Network;

//=============================================================================
// Created:  NLD 2002-01-03
//=============================================================================
class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             ConnectDialog( QWidget* pParent, Network& network, kernel::Logger_ABC& logger );
    virtual ~ConnectDialog();
    //@}

    //! @name Operations
    //@{
    void FillPopupMenu( Q3PopupMenu* menu );
    //@}

private slots:
    //! @name Main methods
    //@{
    void Validate();
    void QuickConnect( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectDialog( const ConnectDialog& );
    ConnectDialog& operator=( const ConnectDialog& );
    //@}

    //! @name Helpers
    //@{
    void LoadDefaultConfig();
    void SaveConfig();
    //@}

private:
    //! @name Member data
    //@{
    Network&            network_;
    kernel::Logger_ABC& logger_;
    QComboBox*   pHostNameComboBox_;
    QSpinBox*    pPortSpinBox_;
    //@}
};

#endif // __ConnectDialog_h_
