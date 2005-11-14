//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectKnowledge.h $
// $Author: Ape $
// $Modtime: 4/11/04 11:15 $
// $Revision: 4 $
// $Workfile: MOS_ObjectKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_ObjectKnowledge_h_
#define __MOS_ObjectKnowledge_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_Object_ABC;
class MOS_Agent;
class MOS_Team;


// =============================================================================
/** @class  MOS_ObjectKnowledge
    @brief  Represents the knowledge a team has of a dynamic object.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_ObjectKnowledge
{
    MT_COPYNOTALLOWED( MOS_ObjectKnowledge );
    friend class MOS_GLTool;
    friend class MOS_ObjectKnowledgePanel;  //$$$$ Beurk.

private:
    enum E_AttributeUpdated
    {
        eUpdated_RealObject               = 0x00000001,
        eUpdated_Relevance                = 0x00000002,
        eUpdated_Localisation             = 0x00000004,  
        eUpdated_PourcentageConstruction  = 0x00000008,
        eUpdated_PourcentageValorisation  = 0x00000010,
        eUpdated_PourcentageContournement = 0x00000020,
        eUpdated_EnPreparation            = 0x00000040,
        eUpdated_IsPerceived              = 0x00000080,
        eUpdated_AutomatePerception       = 0x00000100,
        eUpdated_RiverCrossing            = 0x00000200,
        eUpdated_NBCCloud                 = 0x00000400,
        eUpdated_ROTA                     = 0x00000800,
        eUpdated_LogisticRoute            = 0x00001000,
        eUpdated_Camp                     = 0x00002000,
    };

public:
     MOS_ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg, MOS_Team& owner );
    ~MOS_ObjectKnowledge();

    //! @name Events
    //@{
    void Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    //@}

    //! @name Accessors
    //@{
    uint                 GetID                  () const;
    ASN1T_EnumObjectType GetObjectTypeID        () const;
    bool                 IsValid                ( E_AttributeUpdated ) const;
    const MT_Vector2D&   GetCenter              () const;
    MOS_Object_ABC*      GetRealObject          () const;
    MOS_Team&            GetOwner               () const;
    const T_PointVector& GetPointList           () const;

    // crossing
    uint GetSiteFranchissementLargeur        () const;
    uint GetSiteFranchissementProfondeur     () const;
    uint GetSiteFranchissementVitesseCourant () const;
    bool GetSiteFranchissementBergesAAmenager() const;

    // nbc
    uint GetNuageNBCAgentNbcId () const;

    // rota
    uint                       GetROTADanger   () const;
    const std::vector< uint >& GetROTANBCAgents() const;

    // logistic route
    uint GetLogRouteFlow     () const;
    uint GetLogRouteWidth    () const;
    uint GetLogRouteLength   () const;
    uint GetLogRouteMaxWeight() const;
    bool GetLogRouteEquipped () const;

    // prisoner/refugee camp
    uint GetCampTC2ID() const;

    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MOS_Agent* >                     T_AutomatePerceptionSet;
    typedef T_AutomatePerceptionSet::iterator          IT_AutomatePerceptionSet;
    typedef T_AutomatePerceptionSet::const_iterator    CIT_AutomatePerceptionSet;
    //@}

private:
    MOS_Team&            owner_;
    uint                 nAttrUpdated_;

    uint                 nID_;
    MOS_Object_ABC*  pRealObject_;
    MT_Vector2D          vCenter_;
    ASN1T_EnumObjectType nObjectTypeID_;
    
    T_PointVector        points_;
    
    uint                 nPourcentageConstruction_;
    uint                 nPourcentageValorisation_;
    uint                 nPourcentageContournement_;
    bool                 bEnPreparation_;
    bool                 bIsPerceived_;

    T_AutomatePerceptionSet automatePerceptionSet_;
    uint nRelevance_;
    
    // Attributs spécifiques sites de franchissement - $$$ CRADE POUR L'INSTANT
    uint nSiteFranchissementLargeur_;
    uint nSiteFranchissementProfondeur_;
    uint nSiteFranchissementVitesseCourant_;
    bool nSiteFranchissementBergesAAmenager_;
    
    // Attributs spécifiques nuage NBC
    uint nNuageNBCAgentNbcID_;

    // rota
    uint                nROTADanger_;
    std::vector< uint > rotaNBCAgents_;

    // logistic route
    uint nLogRouteFlow_;
    uint nLogRouteWidth_;
    uint nLogRouteLength_;
    uint nLogRouteMaxWeight_;
    bool bLogRouteEquipped_;

    // prisoner/refugee camp
    uint nCampTC2ID_;
};

#	include "MOS_ObjectKnowledge.inl"

#endif // __MOS_ObjectKnowledge_h_
