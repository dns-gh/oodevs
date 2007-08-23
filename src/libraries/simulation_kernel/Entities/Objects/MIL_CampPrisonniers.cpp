//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampPrisonniers.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_CampPrisonniers.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_CampPrisonniers.h"
#include "MIL_RealObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_Knowledge_ObjectCampPrisonniers.h"
#include "Network/NET_ASN_Messages.h"
#include "xeumeuleu/xml.h"

using namespace xml;

BOOST_CLASS_EXPORT_GUID( MIL_CampPrisonniers, "MIL_CampPrisonniers" )

//-----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampPrisonniers::MIL_CampPrisonniers( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , pTC2_             ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_CampPrisonniers::MIL_CampPrisonniers()
    : MIL_RealObject_ABC()
    , pTC2_             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampPrisonniers::~MIL_CampPrisonniers()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_CampPrisonniers::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & pTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::WriteSpecificAttributes( xml::xostream& xos ) const
{
    assert( pTC2_ );
    xos << start( "specific-attributes" )
            << start( "tc2" )
            << attribute( "id", pTC2_->GetID() )
            << end()
        << end();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool MIL_CampPrisonniers::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
        return false;

    DEC_AutomateDecision* pDecTC2 = diaParameters[ nCurrentParamIdx ].ToUserObject( pDecTC2 );
    if( !pDecTC2 )
        return false;

    if( !pDecTC2->GetAutomate().GetType().IsLogistic() )
        return false;

    pTC2_ = static_cast< MIL_AutomateLOG* >( &pDecTC2->GetAutomate() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    uint nTC2;

    xis >> start( "specific-attributes" )
            >> start( "tc2" )
            >> attribute( "id", nTC2 );

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2 );
    if( !pTC2Tmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is invalid" ); // $$$$ ABL 2007-07-09: error context
    if( !pTC2Tmp->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is not a logistic automate" ); // $$$$ ABL 2007-07-09: error context
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );

    xis >> end()
        >> end();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_CampPrisonniers::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_prisoner_camp )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnCreateObject.specific_attributes.u.prisoner_camp->tc2 );
    if( !pTC2Tmp || !pTC2Tmp->GetType().IsLogistic() )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );

    return MIL_RealObject_ABC::Initialize( asnCreateObject );
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_CampPrisonniers::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectCampPrisonniers( teamKnowing, *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg().m.specific_attributesPresent        = 1;
    asnMsg().specific_attributes.t               = T_ObjectAttributesSpecific_prisoner_camp;
    asnMsg().specific_attributes.u.prisoner_camp = &asnAttributes_;
}
