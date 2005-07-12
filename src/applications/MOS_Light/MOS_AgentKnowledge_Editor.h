//*****************************************************************************
//
// $Created: AGN 03-05-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentKnowledge_Editor.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:30 $
// $Revision: 10 $
// $Workfile: MOS_AgentKnowledge_Editor.h $
//
//*****************************************************************************

#ifndef __MOS_AgentKnowledge_Editor_h_
#define __MOS_AgentKnowledge_Editor_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>
#include <QObjectDefs.h>

class MOS_AgentKnowledge;
class MOS_PointListView;
class MOS_EnemyInfo_Direction;
class MOS_EnemyInfo_Localisation;
class QComboBox;
class MOS_Value;
class QLabel;
class QListView;

//*****************************************************************************
// Created: AGN 03-05-23
//*****************************************************************************
class MOS_AgentKnowledge_Editor
: public QFrame
{
    MT_COPYNOTALLOWED( MOS_AgentKnowledge_Editor )
    Q_OBJECT
    
    friend class MOS_AgentKnowledge;

public:
    MOS_AgentKnowledge_Editor( QWidget* pParent );
    ~MOS_AgentKnowledge_Editor();

    void SetEnemyInfo( MOS_AgentKnowledge* pEditedInfo );
    void Update();

    void Initialize();

public slots:
    void SlotValidate();

private:
    MOS_AgentKnowledge* pEditedInfo_;

    QLabel* pIDLabel_;
    QLabel* pAssociatedAgentLabel_;
    QLabel* pTeamLabel_;
    QLabel* pLevelLabel_;
    QLabel* pWeaponLabel_;
    QLabel* pCategoryLabel_;
    QLabel* pQualifierLabel_;
    QLabel* pSpecializationLabel_;
    QLabel* pMobilityLabel_;
    QLabel* pMissionCapacityLabel_;
    QLabel* pIsHQLabel_;
    QLabel* pPerceptionLevelLabel_;
    QLabel* pMaxPerceptionLevelLabel_;
    QLabel* pEtatOpsLabel_;
    QLabel* pDirectionLabel_;
    QLabel* pSpeedLabel_;
    QLabel* pPositionLabel_;
    QLabel* pRelevanceLabel_;
    QLabel* pPrisonerLabel_;
    QLabel* pSurrenderedLabel_;
    QLabel* pRefugeesManagedLabel_;
    QListView* pAutomatePerceptionsListView_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentKnowledge_Editor.inl"
#endif


#endif // __MOS_AgentKnowledge_Editor_h_