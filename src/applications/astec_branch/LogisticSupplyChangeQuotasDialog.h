//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyChangeQuotasDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 17:14 $
// $Revision: 1 $
// $Workfile: LogisticSupplyChangeQuotasDialog.h $
//
//*****************************************************************************

#ifndef __LogisticSupplyChangeQuotasDialog_h_
#define __LogisticSupplyChangeQuotasDialog_h_

#include "Types.h"

class QListView;
class QPopupMenu;
class QComboBox;
class QComboBoxItem;
class Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class LogisticSupplyChangeQuotasDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( LogisticSupplyChangeQuotasDialog );

public:
     LogisticSupplyChangeQuotasDialog( QWidget* pParent = 0 );
    ~LogisticSupplyChangeQuotasDialog();

    void SetAgent( const Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate     ();
    void Reject       ();
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );
    void onItemRenamed( QListViewItem* item, int col, const QString& text );
    void OnSuppliedChanged( int item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    const Agent* pAgent_;

    QListView*    pQuotas_;
    QComboBox*    pSuppliedComboBox_;
    QPopupMenu*   pTypesMenu_;   
    T_AgentIDMap  automateComboBoxIDs_;
};

#endif // __LogisticSupplyChangeQuotasDialog_h_
