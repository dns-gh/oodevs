//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_SiteFranchissement.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:22 $
// $Revision: 7 $
// $Workfile: MIL_SiteFranchissement.h $
//
//*****************************************************************************

#ifndef __MIL_SiteFranchissement_h_
#define __MIL_SiteFranchissement_h_

#include "MIL.h"
#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_SiteFranchissement : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_SiteFranchissement );

public:
             MIL_SiteFranchissement( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_SiteFranchissement();
    virtual ~MIL_SiteFranchissement();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
    MT_Float GetWidth        () const;
    MT_Float GetDepth        () const;
    MT_Float GetCurrentSpeed () const;
    bool     GetBanksToFitOut() const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool                      Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    //@}

private:
    //! @name Network
    //@
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

private:
    // Specific attributes
    ASN1T_ObjectAttributesCrossingSite asnAttributes_;

    MT_Float rWidth_;
    MT_Float rDepth_;
    MT_Float rCurrentSpeed_;  // Vitesse du courant
    bool     bBanksToFitOut_; // Berges a amenager
};

#include "MIL_SiteFranchissement.inl"

#endif // __MIL_SiteFranchissement_h_
