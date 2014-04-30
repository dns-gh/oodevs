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
    //! @name Functions
    //@{
    // Accessors
    static bool IsEngaged( DEC_Decision_ABC* pAutomate );

    static boost::shared_ptr< MT_Vector2D > GetBarycenter( const DEC_Decision_ABC * automat );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPC( const MIL_Automate& callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetAutomatPionsWithPC( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsWithPC( const MIL_Automate& callerAutomate );

    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC( const MIL_Automate& callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetCommunicationAutomatPionsWithPC( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithoutPC( const MIL_Automate& callerAutomate );
    
    static DEC_Decision_ABC* GetPionPC( const MIL_Automate& callerAutomate );
    static DEC_Decision_ABC* GetPionPCOfAutomate( DEC_Decision_ABC* pAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsMelee( const MIL_Automate& callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetPionsGenie( const MIL_Automate& callerAutomate );
    static std::vector< DEC_Decision_ABC* > GetAutomates( const MIL_Automate& callerAutomate );
    static bool IsParentAutomateEngaged( const MIL_Automate& callerAutomate );
    static int GetRoePopulation( MIL_Automate& callerAutomate );
    static int GetRulesOfEngagementState( MIL_Automate& callerAutomate );

    static std::vector< DEC_Decision_ABC* > GetPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsOfAutomateWithoutPC( const DEC_Decision_ABC* automat );
    static void NotifyRulesOfEngagementStateChanged( MIL_Automate& callerAutomate, int state );
    static void NotifyRulesOfEngagementPopulationStateChanged( MIL_Automate& callerAutomate, int state );

    // Test / debug
    static void DecisionalState( const MIL_Automate& callerAutomate, const std::string& key, const std::string& value );

    // Pion management
    static bool PionChangeAutomate( DEC_Decision_ABC* pion, const DEC_Decision_ABC* superior );

    // Accesseurs sur les fonctions Pion
    static bool IsLogistic( const MIL_Automate& callerAutomat );
    static bool IsPionContaminated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionIntoxicated( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionNeutralized( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPionTransported( DEC_Decision_ABC* pCallerAutomate, DEC_Decision_ABC* pPion );
    static bool IsPointInPionFuseau( const MIL_Automate& callerAutomate, MT_Vector2D* pPoint, DEC_Decision_ABC* pPion );
    static bool MakePionRelievePion( const MIL_Automate& callerAutomate, DEC_Decision_ABC* relieving, DEC_Decision_ABC* relieved );
    static boost::shared_ptr< MT_Vector2D > GetPionPosition( const DEC_Decision_ABC* pion );
    static double GetPerceptionForPion( const DEC_Decision_ABC* pion, boost::shared_ptr< MT_Vector2D >, boost::shared_ptr< MT_Vector2D > );
    static bool CanPionRelievePion( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* relieving, const DEC_Decision_ABC* relieved );
    static bool CanPionConstructObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, const std::string& type );
    static bool CanPionConstructObjectWithLocalisation( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, const std::string& type, const TER_Localisation* localisation );
    static bool CanPionBypassObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static bool CanPionDestroyObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > knowledgeId );
    static bool CanPionDemineObject( const MIL_Automate& callerAutomate, const DEC_Decision_ABC* pion, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLimaForPion( int phaseLine, float distanceBefore, const DEC_Decision_ABC* pion );
    static boost::shared_ptr< MT_Vector2D > ComputePionNearestLocalisationPointInFuseau( const TER_Localisation* location, const DEC_Decision_ABC* pion );
    static unsigned int GetPionEfficiency( const DEC_Decision_ABC* pion, int pionEfficiency );
    static float PionTimeToMoveDistance( const DEC_Decision_ABC* pion, float distance );

    static boost::shared_ptr< MIL_Mission_ABC > GetMission( DEC_Decision_ABC* pAgent );
    static void SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission );
    static boost::shared_ptr< MT_Vector2D > GetDirectionDanger( const boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    static bool IsPionInAutomate( const MIL_Automate& automate, const MIL_AgentPion& pion );
};

#endif // __DEC_AutomateFunctions_h_
