//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ItineraireLogistique.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 16:05 $
// $Revision: 6 $
// $Workfile: MIL_ItineraireLogistique.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ItineraireLogistique.h"
#include "MIL_RealObjectType.h"
#include "Network/NET_ASN_Messages.h"
#include "Knowledge/DEC_Knowledge_ObjectItineraireLogistique.h"

#include "HLA/AttributeIdentifier.h"
#include "HLA/Deserializer.h"
#include "HLA/HLA_UpdateFunctor.h"

BOOST_CLASS_EXPORT_GUID( MIL_ItineraireLogistique, "MIL_ItineraireLogistique" )

//-----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ItineraireLogistique::MIL_ItineraireLogistique( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , bEquipped_        ( false )
    , rWeightSupported_ ( 0. )
    , rWidth_           ( 0. )
    , rLength_          ( 0. )
    , rFlow_            ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ItineraireLogistique::MIL_ItineraireLogistique()
    : MIL_RealObject_ABC()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ItineraireLogistique::~MIL_ItineraireLogistique()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ItineraireLogistique::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file & bEquipped_
         & rWeightSupported_
         & rWidth_
         & rLength_
         & rFlow_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_ItineraireLogistique::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "specific-attributes" );

    archive.WriteField( "max-weight", rWeightSupported_ );
    archive.WriteField( "width"     , rWidth_           );
    archive.WriteField( "length"    , rLength_          );
    archive.WriteField( "flow"      , rFlow_            );
    archive.WriteField( "equipped"  , bEquipped_        );

    archive.EndSection(); // specific-attributes
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::TransformLocalisation
// Created: NLD 2005-02-17
// -----------------------------------------------------------------------------
bool MIL_ItineraireLogistique::TransformLocalisation()
{
    const TER_Localisation& currentLocalisation = GetLocalisation();
    
    if( currentLocalisation.GetType() != TER_Localisation::eLine ||  currentLocalisation.GetPoints().size() < 2 )
        return false;

    TER_Localisation newLocalisation( currentLocalisation );
    newLocalisation.Scale( 1000 );
    UpdateLocalisation( newLocalisation );
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_ItineraireLogistique::Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( diaParameters, nCurrentParamIdx );
    TransformLocalisation();
    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_ItineraireLogistique::Initialize( MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( archive );   

    archive.Section( "specific-attributes" );
    archive.ReadField( "max-weight", rWeightSupported_, CheckValueGreater( 0. ) );
    archive.ReadField( "width"     , rWidth_          , CheckValueGreater( 0. ) );
    archive.ReadField( "length"    , rLength_         , CheckValueGreater( 0. ) );
    archive.ReadField( "flow"      , rFlow_           , CheckValueGreater( 0. ) );
    archive.ReadField( "equipped"  , bEquipped_        );
    archive.EndSection(); // specific-attributes

    TransformLocalisation();
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ItineraireLogistique::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    MIL_RealObject_ABC::Initialize( asnCreateObject );
    if( !TransformLocalisation() )
        return EnumObjectErrorCode::error_invalid_localisation;

    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_itineraire_logistique )
        return EnumObjectErrorCode::error_missing_specific_attributes;


    rWeightSupported_ = asnCreateObject.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
    rWidth_           = asnCreateObject.attributs_specifiques.u.itineraire_logistique->largeur;
    rLength_          = asnCreateObject.attributs_specifiques.u.itineraire_logistique->longueur;
    rFlow_            = asnCreateObject.attributs_specifiques.u.itineraire_logistique->debit;

    return EnumObjectErrorCode::no_error;
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_ItineraireLogistique::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectItineraireLogistique( teamKnowing, *this );
}
   
// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_ItineraireLogistique::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    asnAttributes_.itineraire_equipe  = bEquipped_;
    asnAttributes_.poids_max_supporte = (int)rWeightSupported_;
    asnAttributes_.largeur            = (int)rWidth_;
    asnAttributes_.longueur           = (int)rLength_;
    asnAttributes_.debit              = (int)rFlow_;

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent                = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t                       = T_AttrObjectSpecific_itineraire_logistique;
    asnMsg.GetAsnMsg().attributs_specifiques.u.itineraire_logistique = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::WriteSpecificAttributes
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
void MIL_ItineraireLogistique::WriteSpecificAttributes( NET_ASN_MsgObjectUpdate&   asnMsg )
{
    asnAttributes_.itineraire_equipe  = bEquipped_;
    asnAttributes_.poids_max_supporte = (int)rWeightSupported_;
    asnAttributes_.largeur            = (int)rWidth_;
    asnAttributes_.longueur           = (int)rLength_;
    asnAttributes_.debit              = (int)rFlow_;

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent                = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t                       = T_AttrObjectSpecific_itineraire_logistique;
    asnMsg.GetAsnMsg().attributs_specifiques.u.itineraire_logistique = &asnAttributes_;
}

// =============================================================================
// HLA
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_ItineraireLogistique::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass );
    if( strOption == "equipped" )
        bEquipped_ = true;
    else
        bEquipped_ = false;

    rWeightSupported_ = 0.;
    rWidth_           = 0.;
    rLength_          = 0.;
    rFlow_            = 0.;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Deserialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_ItineraireLogistique::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strEquipped;
        deserializer >> strEquipped;
        if( strEquipped == "equipped" )
            bEquipped_ = true;
        else
            bEquipped_ = false;
    }
    else
        MIL_RealObject_ABC::Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::Serialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_ItineraireLogistique::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "option", ( xAttrToUpdateForHLA_ & eAttrUpdate_SpecificAttributes ) != 0, bEquipped_ ? "equipped" : "not equipped" );
    MIL_RealObject_ABC::Serialize( functor );
}

// =============================================================================
// COLLISION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ItineraireLogistique::CanInteractWith
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
bool MIL_ItineraireLogistique::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    if( !bEquipped_ )
        return false;
    return MIL_RealObject_ABC::CanInteractWith( agent );
}
