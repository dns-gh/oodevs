//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ChangeLogisticLinksDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 15:42 $
// $Revision: 1 $
// $Workfile: MOS_ChangeLogisticLinksDialog.h $
//
//*****************************************************************************

#ifndef __MOS_ChangeLogisticLinksDialog_h_
#define __MOS_ChangeLogisticLinksDialog_h_

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class MOS_Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_ChangeLogisticLinksDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ChangeLogisticLinksDialog );

public:
     MOS_ChangeLogisticLinksDialog( QWidget* pParent = 0 );
    ~MOS_ChangeLogisticLinksDialog();

    void SetAgent( const MOS_Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject  ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MOS_Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    const MOS_Agent* pAgent_;

    QComboBox* pTC2ComboBox_;
    QComboBox* pMaintenanceComboBox_;
    QComboBox* pMedicalComboBox_;
    QComboBox* pSupplyComboBox_;

    T_AgentIDMap maintenanceComboBoxIDs_;
    T_AgentIDMap medicalComboBoxIDs_;
    T_AgentIDMap supplyComboBoxIDs_;
    T_AgentIDMap tc2ComboBoxIDs_;
};

#include "MOS_ChangeLogisticLinksDialog.inl"

#endif // __MOS_ChangeLogisticLinksDialog_h_
