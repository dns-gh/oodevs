// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __DEC_AutomateFunctions_h_
#define __DEC_AutomateFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class MIL_Automate;
class MIL_AgentPion;
class MIL_Mission_ABC;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_AutomateFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    // Accessors
    static boost::shared_ptr< MT_Vector2D > GetBarycenter( const DEC_Decision_ABC* automat );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPC( const DEC_Decision_ABC* callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetAutomatPionsWithPC( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsWithPC( const DEC_Decision_ABC* callerAutomate );

    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC( const DEC_Decision_ABC* callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetCommunicationAutomatPionsWithPC( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithoutPC( const DEC_Decision_ABC* callerAutomate );
    
    static DEC_Decision_ABC* GetPionPC( const DEC_Decision_ABC* callerAutomate );
    static DEC_Decision_ABC* GetPionPCOfAutomate( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsMelee( const DEC_Decision_ABC* callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsGenie( const DEC_Decision_ABC* callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetAutomates( const DEC_Decision_ABC* callerAutomate );
    static bool IsParentAutomateEngaged( const DEC_Decision_ABC* callerAutomate );
    static int GetRoePopulation( DEC_Decision_ABC* callerAutomate );
    static int GetRulesOfEngagementState( DEC_Decision_ABC* callerAutomate );

    static std::vector< DEC_Decision_ABC* > GetPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat );
    static void NotifyRulesOfEngagementStateChanged( DEC_Decision_ABC* callerAutomate, int state );
    static void NotifyRulesOfEngagementPopulationStateChanged( DEC_Decision_ABC* callerAutomate, int state );

    // Pion management
    static bool PionChangeAutomate( DEC_Decision_ABC* pion, const DEC_Decision_ABC* superior );

    // Accesseurs sur les fonctions Pion
    static bool IsLogistic( const DEC_Decision_ABC* callerAutomat );
    static bool IsPionContaminated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionIntoxicated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionNeutralized( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionTransported( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPointInPionFuseau( const DEC_Decision_ABC* callerAutomate, MT_Vector2D* pPoint, DEC_Decision_ABC* pPion );
    static bool MakePionRelievePion( const DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* relieving, DEC_Decision_ABC* relieved );
    static boost::shared_ptr< MT_Vector2D > GetPionPosition( const DEC_Decision_ABC* pion );
    static double GetPerceptionForPion( const DEC_Decision_ABC* pion, boost::shared_ptr< MT_Vector2D >, boost::shared_ptr< MT_Vector2D > );
    static bool CanPionRelievePion( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* relieving, const DEC_Decision_ABC* relieved );
    static bool CanPionConstructObject( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* pion, const std::string& type );
    static bool CanPionConstructObjectWithLocalisation( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* pion, const std::string& type, const TER_Localisation* localisation );
    static bool CanPionBypassObject( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static bool CanPionDestroyObject( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static bool CanPionDemineObject( const DEC_Decision_ABC* callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLimaForPion( int phaseLine, float distanceBefore, const DEC_Decision_ABC* pion );
    static boost::shared_ptr< MT_Vector2D > ComputePionNearestLocalisationPointInFuseau( const TER_Localisation* location, const DEC_Decision_ABC* pion );
    static unsigned int GetPionEfficiency( const DEC_Decision_ABC* pion, int pionEfficiency );
    static float PionTimeToMoveDistance( const DEC_Decision_ABC* pion, float distance );

    static boost::shared_ptr< MIL_Mission_ABC > GetMission( DEC_Decision_ABC* pAgent );
    static void SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    static bool IsPionInAutomate( const MIL_Automate& automate, const MIL_AgentPion& pion );
};

#endif // __DEC_AutomateFunctions_h_
