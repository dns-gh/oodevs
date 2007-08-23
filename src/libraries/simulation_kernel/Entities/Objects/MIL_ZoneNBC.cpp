//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneNBC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:37 $
// $Revision: 9 $
// $Workfile: MIL_ZoneNBC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneNBC.h"

#include "MIL_NbcAgentType.h"
#include "MIL_NbcAgent.h"
#include "MIL_NuageNBC.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Knowledge/DEC_Knowledge_ObjectZoneNBC.h"
#include "Network/NET_ASN_Messages.h"

#include "HLA/AttributeIdentifier.h"
#include "HLA/Deserializer.h"
#include "HLA/HLA_UpdateFunctor.h"

#include "xeumeuleu/xml.h"

using namespace xml;

BOOST_CLASS_EXPORT_GUID( MIL_ZoneNBC, "MIL_ZoneNBC" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneNBC::MIL_ZoneNBC( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , pNbcAgent_        ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZoneNBC::MIL_ZoneNBC()
    : MIL_RealObject_ABC()
    , pNbcAgent_        ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneNBC::~MIL_ZoneNBC()
{
    if( pNbcAgent_ )
        delete pNbcAgent_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_ZoneNBC::serialize
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
template< typename Archive > 
void MIL_ZoneNBC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file & pNbcAgent_;
    file & decontaminatedZones_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::WriteSpecificAttributes( xml::xostream& xos ) const
{
    assert( pNbcAgent_ );
    xos << start( "specific-attributes" )
            << start( "nbc-agent" )
            << attribute( "type", pNbcAgent_->GetType().GetName() )
            << end()
        << end();
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::CreateCloud
// Created: NLD 2006-11-07
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::CreateCloud()
{   
    assert( pNbcAgent_ );
    if( !pNbcAgent_->CanBeVaporized() )
        return;
    MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObjectNuageNBC( GetArmy(), GetLocalisation(), pNbcAgent_->GetType() );
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_ZoneNBC::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx );
    return false;
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_ZoneNBC::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    if( !GetLocalisation().WasACircle() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Localisation of object type 'ZoneNBC' MUST be a circle" ); // $$$$ ABL 2007-07-09: error context

    xis >> start( "specific-attributes" )
            >> start( "nbc-agent" );

    std::string strNbcAgentType_;
    xis >> attribute( "type", strNbcAgentType_ );

    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strNbcAgentType_ );
    if( !pNbcAgentType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown 'AgentNBC' '%s' for NBC object '%d'", strNbcAgentType_.c_str(), GetID() ) ); // $$$$ ABL 2007-07-09: error context

    xis >> end()
        >> end();


    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eLiquid );
    CreateCloud();
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ZoneNBC::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( asnCreateObject.location.type != EnumLocationType::circle )
        return EnumObjectErrorCode::error_invalid_localisation;

    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_nbc_zone )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( asnCreateObject.specific_attributes.u.nbc_zone->nbc_agent );
    if( !pNbcAgentType )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eLiquid );

    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    CreateCloud();
    return EnumObjectErrorCode::no_error;
}
   
//=============================================================================
// EVENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::ProcessAgentInside
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWith( agent ) && !IsAgentInsideDecontaminatedZone( agent ) )
    {
        if( pNbcAgent_->IsContaminating() )
            agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgent_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::ProcessAgentEntering
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    if( CanInteractWith( agent ) && !IsAgentInsideDecontaminatedZone( agent ) )
    {
        if( pNbcAgent_->IsContaminating() )
            agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgent_ );
        if( pNbcAgent_->IsPoisonous() )
            agent.GetRole< PHY_RoleInterface_NBC >().Poison( *pNbcAgent_ );
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pNbcAgent_ );
    asnAttributes_.nbc_agent  = pNbcAgent_->GetType().GetID();

    asnMsg().m.specific_attributesPresent   = 1;
    asnMsg().specific_attributes.t          = T_ObjectAttributesSpecific_nbc_zone;
    asnMsg().specific_attributes.u.nbc_zone = &asnAttributes_;
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_ZoneNBC::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectZoneNBC( teamKnowing, *this );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::GetNbcAgentType
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
const MIL_NbcAgentType& MIL_ZoneNBC::GetNbcAgentType() const
{
    assert( pNbcAgent_ );
    return pNbcAgent_->GetType();
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_ZoneNBC::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass );
    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strOption );
    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eLiquid );    
    return pNbcAgent_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Deserialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strNBC;
        deserializer >> strNBC;
        const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strNBC );
        if( pNbcAgentType )
            pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eLiquid );
    }
    else
        MIL_RealObject_ABC::Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Serialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::Serialize( HLA_UpdateFunctor& functor ) const
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::Serialize( functor );
    functor.Serialize( "option", false, pNbcAgent_->GetType().GetName() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::IsAgentInsideDecontaminatedZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
bool MIL_ZoneNBC::IsAgentInsideDecontaminatedZone( const MIL_Agent_ABC& agent ) const
{
    const MT_Vector2D& vPos = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    for( CIT_LocalisationVector it = decontaminatedZones_.begin(); it != decontaminatedZones_.end(); ++it )
    { 
        if( (**it).IsInside( vPos ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::DecontaminateZone( const TER_Localisation& zone )
{
    if( IsMarkedForDestruction() || GetConstructionPercentage() < 1. || !GetLocalisation().IsIntersecting( zone ) )
        return;
    
    decontaminatedZones_.push_back( new TER_Localisation( zone ) );
}