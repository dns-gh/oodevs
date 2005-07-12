//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:03 $
// $Revision: 15 $
// $Workfile: MOS_ObjectKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_ObjectKnowledge_h_
#define __MOS_ObjectKnowledge_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_IDManager.h"
#include "MOS_Perception_Def.h"

class MOS_ObjectKnowledge_Editor;
class MOS_DynaObject;
class MOS_Agent;

enum E_ObjectTypeID;

//*****************************************************************************
// Created: AGN 03-04-03
//*****************************************************************************
class MOS_ObjectKnowledge
{
    MT_COPYNOTALLOWED( MOS_ObjectKnowledge )
        
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
        eUpdated_AutomatePerception       = 0x00000100
    };

    friend class MOS_ObjectKnowledge_ListView_Item;

public:
     MOS_ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& asnMsg );
    ~MOS_ObjectKnowledge();

    //! @name Events
    //@{
    void Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg );
    //@}

    //! @name Editor
    //@{
    void                               SetListViewItem( MOS_ObjectKnowledge_ListView_Item* pEditor );
    MOS_ObjectKnowledge_ListView_Item* GetListViewItem() const;
    //@}

    //! @name Accessors
    //@{
    uint               GetID       () const;
    bool               IsValid     ( E_AttributeUpdated ) const;
    const MT_Vector2D& GetCenter   () const;
    MOS_DynaObject*    GetRealObject() const;
    //@}

    void Draw();

private:
    //! @name Types
    //@{
    typedef std::set< MOS_Agent* >                     T_AutomatePerceptionSet;
    typedef T_AutomatePerceptionSet::iterator          IT_AutomatePerceptionSet;
    typedef T_AutomatePerceptionSet::const_iterator    CIT_AutomatePerceptionSet;
    //@}

private:
    MOS_ObjectKnowledge_ListView_Item* pListViewItem_;

    uint nAttrUpdated_;

    uint              nID_;
    MOS_DynaObject*   pRealObject_;
    MT_Vector2D       vCenter_;
    ASN1T_EnumObjectType nObjectTypeID_;
    
    T_PointVector     points_;
    
    uint nPourcentageConstruction_;
    uint nPourcentageValorisation_;
    uint nPourcentageContournement_;
    bool bEnPreparation_;
    bool bIsPerceived_;

    T_AutomatePerceptionSet automatePerceptionSet_;
    uint nRelevance_;
    
    // Attributs spécifiques sites de franchissement - $$$ CRADE POUR L'INSTANT
    bool bAttrSiteFranchissementPresent_;
    uint nSiteFranchissementLargeur_;
    uint nSiteFranchissementProfondeur_;
    uint nSiteFranchissementVitesseCourant_;
    bool nSiteFranchissementBergesAAmenager_;
    
    // Attributs spécifiques nuage NBC
    bool bAttrNuageNBCPresent_;
    uint nNuageNBCAgentNbcID_;

    // Attributs spécifiques ROTA
    bool                bAttrRotaPresent_;
    uint                nRotaDanger_;
    std::vector< uint > rotaNbcAgents_;

    // Attributs spécifiques ITI LOG
    bool bAttrItiLogPresent_;
    bool bItiLogEquipped_;
    uint nItiLogistiqueFlow_;
    uint nItiLogistiqueWidth_;
    uint nItiLogistiqueLength_;
    uint nItiLogistiqueMaxWeight_;
};

typedef std::vector< MOS_ObjectKnowledge* >      T_ObjectKnowledgeVector;
typedef T_ObjectKnowledgeVector::iterator        IT_ObjectKnowledgeVector;
typedef T_ObjectKnowledgeVector::const_iterator  CIT_ObjectKnowledgeVector;

#include "MOS_ObjectKnowledge.inl"


#endif // __MOS_ObjectKnowledge_h_
