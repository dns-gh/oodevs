//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObject.h $
// $Author: Age $
// $Modtime: 5/04/05 11:53 $
// $Revision: 7 $
// $Workfile: MOS_DynaObject.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObject_h_
#define __MOS_DynaObject_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_Team;


// =============================================================================
/** @class  MOS_DynaObject
    @brief  Represents a dynamic object of the simulation, ie. minefield,
            fortification, tank ditch... 
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_DynaObject
{
    MT_COPYNOTALLOWED( MOS_DynaObject );
    friend class MOS_GLTool;

public:
     MOS_DynaObject();
    ~MOS_DynaObject();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( const ASN1T_MsgObjectCreation& asnMsg );
    static void InitializeObjectIds( MT_XXmlInputArchive& classeIds );
    //@}

    //-------------------------------------------------------------------------
    /** @name Update */
    //-------------------------------------------------------------------------
    //@{
    void Update( const ASN1T_MsgObjectUpdate& asnMsg );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    uint GetID() const;
    ASN1T_EnumObjectType GetType() const;
    const MOS_Team& GetTeam() const;
    bool IsPrepared() const;
    MT_Float GetConstructionPercentage() const;
    MT_Float GetValorizationPercentage() const;
    MT_Float GetBypassConstructionPercentage() const;
    const MT_Vector2D& GetCenter() const;
    const T_PointVector& GetPointList() const;
    ASN1T_EnumTypeLocalisation GetLocationType() const;

    bool AreAttrSiteFranchissementPresent    () const;
    uint GetSiteFranchissementLargeur        () const;
    uint GetSiteFranchissementProfondeur     () const;
    uint GetSiteFranchissementVitesseCourant () const;
    bool GetSiteFranchissementBergesAAmenager() const;

	bool AreAttrTC2Present () const;
	uint GetTC2ID		   () const;
    
    bool AreAttrNuageNBCPresent() const;
    uint GetNuageNBCAgentNbcId () const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Modifiers */
    //-------------------------------------------------------------------------
    //@{
    void SetType( ASN1T_EnumObjectType nType );
    void SetID( uint nID );
    void SetTeam( MOS_Team& team );
    void SetLocalisation( ASN1T_EnumTypeLocalisation nType, const T_PointVector& pointVector );

    void SetFranchissementParameters( uint nSiteFranchissementLargeur, uint nSiteFranchissementProfondeur,
                                      uint nSiteFranchissementVitesseCourant, bool nSiteFranchissementBergesAAmenager );
    
    void SetNBCParameter( uint nNuageNBCAgentNbcID );

	void SetCampParameter( uint nTC2ID_ );
    //@}

    void ReadODB( MT_XXmlInputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

    static MOS_IDManager& GetIDManagerForObjectType( ASN1T_EnumObjectType nType );

private:
    ASN1T_EnumObjectType nType_;
    uint                 nID_;
    MOS_Team*            pTeam_;

    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    T_PointVector pointVector_;
    MT_Vector2D center_;
    
    MT_Float rConstructionPercentage_;
    MT_Float rValorizationPercentage_;
    MT_Float rBypassConstructionPercentage_;

    bool bPrepared_;

    // Attributs spécifiques sites de franchissement - $$$ CRADE POUR L'INSTANT
    bool bAttrSiteFranchissementPresent_;
    uint nSiteFranchissementLargeur_;
    uint nSiteFranchissementProfondeur_;
    uint nSiteFranchissementVitesseCourant_;
    bool nSiteFranchissementBergesAAmenager_;

    // Attributs spécifiques nuage NBC
    bool bAttrNuageNBCPresent_;
    uint nNuageNBCAgentNbcID_;

	// Attributs spécifiques camps
	bool bAttrTC2Present_;
	uint nTC2ID_;

public:
    //! @name Types
    //@{
    typedef std::map< ASN1T_EnumObjectType, unsigned int >  T_ObjectIDs;
    typedef std::map< unsigned int, MOS_IDManager* >        T_Managers;
    //@}
public:
    static T_ObjectIDs objectIds_;
    static T_Managers managers_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject.inl"
#endif

#endif // __MOS_DynaObject_h_
