//****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Math�matiques Appliqu�es SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************

#ifndef __DisconnectDialog_h_
#define __DisconnectDialog_h_

#include "clients_gui/Types.h"

class Network;

//=============================================================================
/**
*   @class  DisconnectDialog
*   @brief  Disconnect dialog
*/
// Created:  NLD 2002-01-03
//=============================================================================
class DisconnectDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             DisconnectDialog( QWidget* pParent, Network& network );
    virtual ~DisconnectDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisconnectDialog( const DisconnectDialog& );
    DisconnectDialog& operator=( const DisconnectDialog& );
    //@}

private:
    //! @name Member data
    //@{
    Network& network_;
    //@}
};

#endif // __DisconnectDialog_h_
