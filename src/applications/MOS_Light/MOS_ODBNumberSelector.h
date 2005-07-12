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
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBNumberSelector.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_ODBNumberSelector.h $
//
//*****************************************************************************

#ifndef __MOS_ODBNumberSelector_h_
#define __MOS_ODBNumberSelector_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class QSpinBox;

class MOS_ODBNumberSelector : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ODBNumberSelector );

public:
     MOS_ODBNumberSelector( uint* pNumber, QWidget* pParent = 0 );
    ~MOS_ODBNumberSelector();

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject();
    //@}

private:
    QComboBox*  pHostNameComboBox_;
    QSpinBox*   pPortSpinBox_;
    uint*       pNumber_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_ODBNumberSelector.inl"
#endif

#endif // __MOS_ODBNumberSelector_h_
