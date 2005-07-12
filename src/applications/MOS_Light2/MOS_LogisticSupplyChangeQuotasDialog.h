//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogisticSupplyChangeQuotasDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 17:14 $
// $Revision: 1 $
// $Workfile: MOS_LogisticSupplyChangeQuotasDialog.h $
//
//*****************************************************************************

#ifndef __MOS_LogisticSupplyChangeQuotasDialog_h_
#define __MOS_LogisticSupplyChangeQuotasDialog_h_

#include "MOS_Types.h"

class QListView;
class QPopupMenu;
class QComboBox;
class QListViewItem;
class MOS_Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_LogisticSupplyChangeQuotasDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_LogisticSupplyChangeQuotasDialog );

public:
     MOS_LogisticSupplyChangeQuotasDialog( QWidget* pParent = 0 );
    ~MOS_LogisticSupplyChangeQuotasDialog();

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
    const MOS_Agent* pAgent_;

    QListView*    pQuotas_;
    QComboBox*    pAutomateChangedComboBox_;
    QPopupMenu*   pTypesMenu_;   
    T_AgentIDMap  automateComboBoxIDs_;
};

#include "MOS_LogisticSupplyChangeQuotasDialog.inl"

#endif // __MOS_LogisticSupplyChangeQuotasDialog_h_
