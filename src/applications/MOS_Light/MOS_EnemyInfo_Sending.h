//*****************************************************************************
//
// $Created: AGN 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Sending.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_EnemyInfo_Sending.h $
//
//*****************************************************************************

#ifndef __MOS_EnemyInfo_Sending_h_
#define __MOS_EnemyInfo_Sending_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>
#include <QObjectDefs.h>

class MOS_AgentListView;
class MOS_EnemyInfo;

//*****************************************************************************
// Created: AGN 03-05-21
//*****************************************************************************
class MOS_EnemyInfo_Sending
: public QFrame
{
    MT_COPYNOTALLOWED( MOS_EnemyInfo_Sending )
    Q_OBJECT
    
public:
    MOS_EnemyInfo_Sending( QWidget* pParent );
    ~MOS_EnemyInfo_Sending();

    void SetEnemyInfo( MOS_EnemyInfo* pEditedInfo );
//    bool IsEditing() const;

private:
    void Initialize();
    void Terminate ();

private slots:
    void SlotValidate();
    void SlotCancel  ();    

private:
    MOS_AgentListView* pAgentView_;
    MOS_EnemyInfo* pEditedInfo_;

    /*
    QLabel* pUnitName_;
    QComboBox* pConfirmationBox_;
    QComboBox* pHierarchyBox_;
    QComboBox* pAttitudeBox_;
    QComboBox* pWeaponBox_;
    QComboBox* pCategoryBox_;
    QComboBox* pQualifierBox_;
    QComboBox* pSpecializationBox_;
    QComboBox* pMobilityBox_;

    MOS_EnemyInfo_Direction*      pDirectionEditor_;
    MOS_EnemyInfo_Localisation*   pLocationEditor_;
    MOS_Value*                pSpeed_;
    */
};

#ifdef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Sending.inl"
#endif


#endif // __MOS_EnemyInfo_Sending_h_