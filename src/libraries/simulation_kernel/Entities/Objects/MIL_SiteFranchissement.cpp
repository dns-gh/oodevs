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

#include "xeumeuleu/xml.h"



BOOST_CLASS_EXPORT_GUID( MIL_SiteFranchissement, "MIL_SiteFranchissement" )

//=============================================================================
// CONSTRUCTOR / DESTRUCTOR
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_SiteFranchissement::MIL_SiteFranchissement( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , rWidth_           ( 0 )
    , rDepth_           ( 0 )
    , rCurrentSpeed_    ( 0 )
    , bBanksToFitOut_   ( true )   
{
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_SiteFranchissement::MIL_SiteFranchissement()
    : MIL_RealObject_ABC()
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
void MIL_SiteFranchissement::WriteSpecificAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "specific-attributes" )
            << xml::content( "width", rWidth_ )
            << xml::content( "depth", rDepth_ )
            << xml::content( "speed", rCurrentSpeed_ )
            << xml::content( "construction-needed", bBanksToFitOut_ )
        << xml::end(); // specific-attributes
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_SiteFranchissement::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    return MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx );
}

//-----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_SiteFranchissement::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    xis >> xml::start( "specific-attributes" )
            >> xml::content( "width", rWidth_ )
            >> xml::content( "depth", rDepth_ )
            >> xml::content( "speed", rCurrentSpeed_ )
            >> xml::content( "construction-needed", bBanksToFitOut_ )
        >> xml::end();

    if( rWidth_ <= 0. )
        xis.error( "rWidth_ is not greater than 0 " );

    if( rDepth_ <= 0 )
        xis.error( "rDepth_ is not greater than 0 " );

    if( rCurrentSpeed_ < 0 )
        xis.error( "rCurrentSpeed_ is not greater or equal to 0 " );
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteFranchissement::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_SiteFranchissement::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_crossing_site )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    rWidth_         = asnCreateObject.specific_attributes.u.crossing_site->width;
    rDepth_         = asnCreateObject.specific_attributes.u.crossing_site->depth;
    rCurrentSpeed_  = asnCreateObject.specific_attributes.u.crossing_site->flow_rate;
    bBanksToFitOut_ = asnCreateObject.specific_attributes.u.crossing_site->banks_require_fitting != false;

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
    asnAttributes_.width                 = (int)rWidth_;
    asnAttributes_.depth                 = (int)rDepth_;
    asnAttributes_.flow_rate             = (int)rCurrentSpeed_;
    asnAttributes_.banks_require_fitting = bBanksToFitOut_;

    asnMsg().m.specific_attributesPresent        = 1;
    asnMsg().specific_attributes.t               = T_ObjectAttributesSpecific_crossing_site;
    asnMsg().specific_attributes.u.crossing_site = &asnAttributes_;
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
bool MIL_SiteFranchissement::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    rWidth_ = rDepth_ = rCurrentSpeed_ = 0; // $$$$ AGE 2004-12-01: 
    return MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass );
}

