//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Editor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_EnemyInfo_Editor.h $
//
//*****************************************************************************

#ifndef __MOS_EnemyInfo_Editor_h_
#define __MOS_EnemyInfo_Editor_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>
#include <QObjectDefs.h>

class MOS_EnemyInfo;
class MOS_PointListView;
class MOS_EnemyInfo_Direction;
class MOS_EnemyInfo_Localisation;
class QComboBox;
class MOS_Value;
class QLabel;

//*****************************************************************************
// Created: AGN 03-05-20
//*****************************************************************************
class MOS_EnemyInfo_Editor
: public QFrame
{
    MT_COPYNOTALLOWED( MOS_EnemyInfo_Editor )
    Q_OBJECT
    
public:
    MOS_EnemyInfo_Editor( QWidget* pParent );
    ~MOS_EnemyInfo_Editor();

    void SetEnemyInfo( MOS_EnemyInfo* pEditedInfo );
    bool IsEditing() const;

private:
    void Initialize();

private slots:
    void SlotValidate();
    void SlotCancel  ();    

private:
    MOS_EnemyInfo* pEditedInfo_;

    QComboBox* pTeamBox_;
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
};

#ifdef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Editor.inl"
#endif


#endif // __MOS_EnemyInfo_Editor_h_