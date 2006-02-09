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
#include "OptionalValue.h"

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
    uint                 nID_;
    MT_Vector2D          vCenter_;
    ASN1T_EnumObjectType nObjectTypeID_;

    OptionalValue< MOS_Object_ABC* >     pRealObject_;
    OptionalValue< T_PointVector >       points_;
    OptionalValue< std::string >         strPos_;
    OptionalValue< uint >                nPourcentageConstruction_;
    OptionalValue< uint >                nPourcentageValorisation_;
    OptionalValue< uint >                nPourcentageContournement_;
    OptionalValue< bool >                bEnPreparation_;
    OptionalValue< bool >                bIsPerceived_;
    OptionalValue< uint >                nRelevance_;

    OptionalValue< T_AutomatePerceptionSet > automatePerceptionSet_;
    
    
    // Attributs spécifiques sites de franchissement - $$$ CRADE POUR L'INSTANT
    OptionalValue< uint > nSiteFranchissementLargeur_;
    OptionalValue< uint > nSiteFranchissementProfondeur_;
    OptionalValue< uint > nSiteFranchissementVitesseCourant_;
    OptionalValue< bool > nSiteFranchissementBergesAAmenager_;
    
    // Attributs spécifiques nuage NBC
    OptionalValue< uint > nNuageNBCAgentNbcID_;

    // rota
    OptionalValue< uint >                nROTADanger_;
    OptionalValue< std::vector< uint > > rotaNBCAgents_;

    // logistic route
    OptionalValue< uint > nLogRouteFlow_;
    OptionalValue< uint > nLogRouteWidth_;
    OptionalValue< uint > nLogRouteLength_;
    OptionalValue< uint > nLogRouteMaxWeight_;
    OptionalValue< bool > bLogRouteEquipped_;

    // prisoner/refugee camp
    OptionalValue< uint > nCampTC2ID_;
};

#	include "MOS_ObjectKnowledge.inl"

#endif // __MOS_ObjectKnowledge_h_
