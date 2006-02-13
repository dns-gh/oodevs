//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ChangeLogisticLinksDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 15:42 $
// $Revision: 1 $
// $Workfile: ChangeLogisticLinksDialog.h $
//
//*****************************************************************************

#ifndef __ChangeLogisticLinksDialog_h_
#define __ChangeLogisticLinksDialog_h_

#include "Types.h"
#include <QDialog.h>

class QComboBox;
class Agent;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ChangeLogisticLinksDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ChangeLogisticLinksDialog );

public:
     ChangeLogisticLinksDialog( QWidget* pParent = 0 );
    ~ChangeLogisticLinksDialog();

    void SetAgent( Agent& agent );

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
    typedef std::map< uint, const Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}
   
private:
    Agent* pAgent_;

    QComboBox* pTC2ComboBox_;
    QComboBox* pMaintenanceComboBox_;
    QComboBox* pMedicalComboBox_;
    QComboBox* pSupplyComboBox_;

    T_AgentIDMap maintenanceComboBoxIDs_;
    T_AgentIDMap medicalComboBoxIDs_;
    T_AgentIDMap supplyComboBoxIDs_;
    T_AgentIDMap tc2ComboBoxIDs_;
};

#endif // __ChangeLogisticLinksDialog_h_
