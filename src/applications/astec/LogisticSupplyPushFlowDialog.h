//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyPushFlowDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 16:40 $
// $Revision: 1 $
// $Workfile: LogisticSupplyPushFlowDialog.h $
//
//*****************************************************************************

#ifndef __LogisticSupplyPushFlowDialog_h_
#define __LogisticSupplyPushFlowDialog_h_

#include "Types.h"
#include <QDialog.h>

class QListView;
class QPopupMenu;
class QComboBox;
class QListViewItem;
class Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class LogisticSupplyPushFlowDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( LogisticSupplyPushFlowDialog );

public:
     LogisticSupplyPushFlowDialog( QWidget* pParent = 0 );
    ~LogisticSupplyPushFlowDialog();

    void SetAgent( const Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate     ();
    void Reject       ();
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );
    void OnSuppliedChanged( int item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    bool bInitialized_;
    const Agent* pAgent_;
    
    QListView*    pStocks_;
    QComboBox*    pSuppliedComboBox_;
    QPopupMenu*   pTypesMenu_;   
    T_AgentIDMap  automateComboBoxIDs_;
};

#endif // __LogisticSupplyPushFlowDialog_h_
