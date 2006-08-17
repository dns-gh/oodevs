//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_SiteFranchissement.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:22 $
// $Revision: 7 $
// $Workfile: MIL_SiteFranchissement.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_SiteFranchissement.h"
#include "MIL_RealObjectType.h"
#include "Network/NET_ASN_Messages.h"
#include "Knowledge/DEC_Knowledge_ObjectSiteFranchissement.h"

#include "hla/Deserializer.h"

BOOST_CLASS_EXPORT_GUID( MIL_SiteFranchissement, "MIL_SiteFranchissement" )

//=============================================================================
// CONSTRUCTOR / DESTRUCTOR
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_SiteFranchissement::MIL_SiteFranchissement()
    : MIL_RealObject_ABC( MIL_RealObjectType::siteFranchissement_ )
    , rWidth_           ( 0 )
    , rDepth_           ( 0 )
    , rCurrentSpeed_    ( 0 )
    , bBanksToFitOut_   ( true )   
{
}

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_SiteFranchissement::~MIL_SiteFranchissement()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_SiteFranchissement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file & rWidth_
         & rDepth_
         & rCurrentSpeed_
         & bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_SiteFranchissement::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteField( "Largeur"        , rWidth_ );
    archive.WriteField( "Profondeur"     , rDepth_ );
    archive.WriteField( "VitesseCourant" , rCurrentSpeed_ );
    archive.WriteField( "BergesAAmenager", bBanksToFitOut_ );   
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_SiteFranchissement::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    return MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx );
}


//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_SiteFranchissement::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    archive.ReadField( "Largeur"        , rWidth_, CheckValueGreater( 0. ) );
    archive.ReadField( "Profondeur"     , rDepth_, CheckValueGreater( 0. ) );
    archive.ReadField( "VitesseCourant" , rCurrentSpeed_, CheckValueGreaterOrEqual( 0. ) );
    archive.ReadField( "BergesAAmenager", bBanksToFitOut_ );   
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_SiteFranchissement::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_site_franchissement )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    rWidth_         = asnCreateObject.attributs_specifiques.u.site_franchissement->largeur;
    rDepth_         = asnCreateObject.attributs_specifiques.u.site_franchissement->profondeur;
    rCurrentSpeed_  = asnCreateObject.attributs_specifiques.u.site_franchissement->vitesse_courant;
    bBanksToFitOut_ = asnCreateObject.attributs_specifiques.u.site_franchissement->berges_a_amenager != false;

    return EnumObjectErrorCode::no_error;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_SiteFranchissement::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    asnAttributes_.largeur           = (int)rWidth_;
    asnAttributes_.profondeur        = (int)rDepth_;
    asnAttributes_.vitesse_courant   = (int)rCurrentSpeed_;
    asnAttributes_.berges_a_amenager = bBanksToFitOut_;

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t                     = T_AttrObjectSpecific_site_franchissement;
    asnMsg.GetAsnMsg().attributs_specifiques.u.site_franchissement = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::CreateKnowledge
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_SiteFranchissement::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectSiteFranchissement( teamKnowing, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_SiteFranchissement::Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    rWidth_ = rDepth_ = rCurrentSpeed_ = 0; // $$$$ AGE 2004-12-01: 
    return MIL_RealObject_ABC::Initialize( strOption, strExtra, rCompletion, rMining, rBypass );
}

