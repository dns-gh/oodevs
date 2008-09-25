//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ItineraireLogistique.h $
// $Author: Nld $
// $Modtime: 27/04/05 12:08 $
// $Revision: 4 $
// $Workfile: MIL_ItineraireLogistique.h $
//
//*****************************************************************************

#ifndef __MIL_ItineraireLogistique_h_
#define __MIL_ItineraireLogistique_h_

#include "MIL.h"
#include "MIL_RealObject_ABC.h"
#include "game_asn/Simulation.h"

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ItineraireLogistique : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ItineraireLogistique );

public:
             MIL_ItineraireLogistique( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ItineraireLogistique();
    virtual ~MIL_ItineraireLogistique();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool                      Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( xml::xistream& xis );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asn );
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize ( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    virtual void Serialize  ( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Specific
    //@{
            MT_Float GetWeightSupported  () const;
            MT_Float GetWidth            () const;
            MT_Float GetLength           () const;
            MT_Float GetFlow             () const;
            bool     IsEquipped          () const;
            void     Equip               ();
    //@}

private:               
    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( xml::xostream& xos ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectUpdate&   asnMsg );
            bool TransformLocalisation  ();
    //@}

    //! @name Tools
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    //@}

private:
    // Specific attributes
    ASN1T_ObjectAttributesLogisticRoute asnAttributes_;
    bool                                 bEquipped_;
    MT_Float                             rWeightSupported_;
    MT_Float                             rWidth_;
    MT_Float                             rLength_;
    MT_Float                             rFlow_;
};

#include "MIL_ItineraireLogistique.inl"

#endif // __MIL_ItineraireLogistique_h_
