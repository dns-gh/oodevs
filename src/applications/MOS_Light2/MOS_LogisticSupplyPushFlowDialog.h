//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogisticSupplyPushFlowDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 16:40 $
// $Revision: 1 $
// $Workfile: MOS_LogisticSupplyPushFlowDialog.h $
//
//*****************************************************************************

#ifndef __MOS_LogisticSupplyPushFlowDialog_h_
#define __MOS_LogisticSupplyPushFlowDialog_h_

#include "MOS_Types.h"
#include <QDialog.h>

class QListView;
class QPopupMenu;
class QComboBox;
class QListViewItem;
class MOS_Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_LogisticSupplyPushFlowDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LogisticSupplyPushFlowDialog );

public:
     MOS_LogisticSupplyPushFlowDialog( QWidget* pParent = 0 );
    ~MOS_LogisticSupplyPushFlowDialog();

    void SetAgent( const MOS_Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate     ();
    void Reject       ();
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MOS_Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    bool bInitialized_;
    const MOS_Agent* pAgent_;
    
    QListView*    pStocks_;
    QComboBox*    pAutomateChangedComboBox_;
    QPopupMenu*   pTypesMenu_;   
    T_AgentIDMap  automateComboBoxIDs_;
};

#include "MOS_LogisticSupplyPushFlowDialog.inl"

#endif // __MOS_LogisticSupplyPushFlowDialog_h_
