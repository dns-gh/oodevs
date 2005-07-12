//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_SIMControl.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_SIMControl.h $
//
//*****************************************************************************

#ifndef __MOS_SIMControl_h_
#define __MOS_SIMControl_h_

#include "MOS_Types.h"

#include <QFrame.h>
#include <QToolButton.h>

class MOS_Value;
class MOS_ConnectDialog;
class MOS_DisconnectDialog;

//*****************************************************************************
// Created: FBD 03-01-14
//*****************************************************************************
class MOS_SIMControl : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_SIMControl );

public:
    explicit MOS_SIMControl( QWidget* pParent );
    virtual ~MOS_SIMControl();
    //-------------------------------------------------------------------------
    /** @name accessors*/
    //-------------------------------------------------------------------------
    //@{
    void SetInitSpeed();
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void NotifyConnection( bool bConnected );
    void NotifyStatePause( bool bPause );
    //@}


protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotConnection();
    void SlotPause();
    void SlotSpeedChange();
    //@}

private:
    MOS_Value*              pSpeed_;
    QToolButton*            pConnectButton_;
    QToolButton*            pStateRunButton_;
    MOS_ConnectDialog*      pConnectDlg_;
    MOS_DisconnectDialog*   pDisconnectDlg_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_SIMControl.inl"
#endif


#endif // __MOS_SIMControl_h_