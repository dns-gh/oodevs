//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject.h $
// $Author: Nld $
// $Modtime: 27/04/05 14:54 $
// $Revision: 18 $
// $Workfile: MOS_DynaObject.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObject_h_
#define __MOS_DynaObject_h_

#include "MOS_Types.h"

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

enum E_ObjectTypeID;

class MOS_DynaObject_ListViewItem;

//=============================================================================
// Created: NLD 2003-01-29
//=============================================================================
class MOS_DynaObject
{
    MT_COPYNOTALLOWED( MOS_DynaObject );

public:
    MOS_DynaObject();
    virtual ~MOS_DynaObject();


    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( const ASN1T_MsgObjectCreation& asnMsg );
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Update( const ASN1T_MsgObjectUpdate& asnMsg );
    void Draw  ();
    //@}

    //-------------------------------------------------------------------------
    /** @name GUI                                                            */
    //-------------------------------------------------------------------------
    //@{
    void SetViewItem( MOS_DynaObject_ListViewItem* pItem );
    MOS_DynaObject_ListViewItem* GetViewItem();
    //@}


    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    uint GetID() const;
    ASN1T_EnumObjectType GetType() const;
    bool IsPrepared() const;
    MT_Float GetConstructionPercentage() const;
    MT_Float GetValorizationPercentage() const;
    MT_Float GetBypassConstructionPercentage() const;
    const MT_Vector2D& GetCenter() const;
    uint  GetIDPlanfie() const;

    bool AreAttrSiteFranchissementPresent    () const;
    uint GetSiteFranchissementLargeur        () const;
    uint GetSiteFranchissementProfondeur     () const;
    uint GetSiteFranchissementVitesseCourant () const;
    bool GetSiteFranchissementBergesAAmenager() const;

    bool AreAttrNuageNBCPresent() const;
    uint GetNuageNBCAgentNbcId () const;

    bool                       AreAttrRotaPresent() const;
    uint                       GetRotaDanger     () const;
    const std::vector< uint >& GetRotaNbcAgents  () const;

    bool AreAttrItineraireLogistiquePresent() const;
    bool IsItineraireLogistiqueEquipped    () const;
    uint GetItineraireLogistiqueFlow       () const;
    uint GetItineraireLogistiqueWidth      () const;
    uint GetItineraireLogistiqueLength     () const;
    uint GetItineraireLogistiqueMaxWeight  () const;
    //@}

private:
    ASN1T_EnumObjectType   nType_;
    uint             nID_;

    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    T_PointVector pointVector_;
    MT_Vector2D center_;
    
    MT_Float rConstructionPercentage_;
    MT_Float rValorizationPercentage_;
    MT_Float rBypassConstructionPercentage_;
    uint     nIDPlanifie_;

    bool bPrepared_;

    MOS_DynaObject_ListViewItem* pItem_;

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

    // Attributs spécifiques 
    bool bAttrItineraireLogisticPresent_;
    bool bItineraireLogistiqueEquipped_;
    uint nItineraireLogistiqueFlow_;
    uint nItineraireLogistiqueWidth_;
    uint nItineraireLogistiqueLength_;
    uint nItineraireLogistiqueMaxWeight_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject.inl"
#endif

#endif // __MOS_DynaObject_h_
