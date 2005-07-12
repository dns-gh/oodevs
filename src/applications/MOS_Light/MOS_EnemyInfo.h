//*****************************************************************************
// 
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_EnemyInfo.h $
// 
//*****************************************************************************

#ifndef __MOS_EnemyInfo_h_
#define __MOS_EnemyInfo_h_

#include "MOS_Agent.h"
#include "MOS_Attr_Def.h"

//*****************************************************************************
// Created: AGN 03-05-20
//*****************************************************************************
class MOS_EnemyInfo
{
    friend class MOS_EnemyInfo_Editor;
    friend class MOS_EnemyInfo_Sending;

public:
    MOS_EnemyInfo( int nID );
    ~MOS_EnemyInfo();

    int nId_;

    void SetName( const char* szName );
    void Draw();

    void AddReferecingAutomata( const MOS_Agent& agent );
    void RemoveReferecingAutomata( const MOS_Agent& agent );

private:
    std::string strName_;
    T_PointVector pointList_;
    
    enum E_ConfirmationLevel
    {
        eConfirmationLevel_Real = 0,
        eConfirmationLevel_Estd = 2,
        eConfirmationLevel_None = 6
    };
    
    enum E_HierarchicalLevel
    {
        eHierarchicalLevel_Squad = 0,   // Groupe
        eHierarchicalLevel_Section,     // Section
        eHierarchicalLevel_Team,        // Equipe
        eHierarchicalLevel_AlliedForces,// Force alliée
        eHierarchicalLevel_FieldArmy,   // Armée
        eHierarchicalLevel_ArmyGroup,   // Front
        eHierarchicalLevel_Division,    // Division
        eHierarchicalLevel_Corps,       // Corps d'armée
        eHierarchicalLevel_Regiment,    // Régiment
        eHierarchicalLevel_Brigade,     // Brigade
        eHierarchicalLevel_Compagny,    // Compagnie
        eHierarchicalLevel_Battalion,   // Bataillon
        eHierarchicalLevel_None
    };
    
    enum E_Attitude
    {
        eAttitude_Static = 0,
        eAttitude_OffensiveMove,
        eAttitude_DefensiveMove,
        eAttitude_NotDefined
    };


    E_ConfirmationLevel    nConfirmationLevel_;
    E_HierarchicalLevel    nHierarchicalLevel_;
    E_Attitude             nAttitude_;
    int                    nCamp_;
    int                    nSpeed_;
    MT_Vector2D            startDir_;
    MT_Vector2D            endDir_;
    E_NatureWeaponType         nWeapon_;
    E_NatureSpecializationType nSpecialization_;
    E_NatureQualifierType      nQualifier_;
    E_NatureCategoryType       nCategory_;
    E_NatureMobilityType       nMobility_;

    MOS_Agent::T_AgentConstPtrSet referencingAutomataSet_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_EnemyInfo.inl"
#endif


#endif // __MOS_EnemyInfo_h_