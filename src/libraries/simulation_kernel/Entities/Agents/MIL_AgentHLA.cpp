// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentHLA.cpp $
// $Author: Jvt $
// $Modtime: 25/03/05 12:39 $
// $Revision: 16 $
// $Workfile: MIL_AgentHLA.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentHLA.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Roles/Refugee/PHY_RoleHLA_Refugee.h"
#include "Roles/Surrender/PHY_RoleHLA_Surrender.h"
#include "Roles/Dotations/PHY_RoleHLA_Dotations.h"
#include "Roles/Location/PHY_RoleHLA_Location.h"
#include "Roles/Posture/PHY_RoleHLA_Posture.h"
#include "Roles/Composantes/PHY_RoleHLA_Composantes.h"
#include "Roles/NBC/PHY_RoleHLA_NBC.h"
#include "Roles/Communications/PHY_RoleHLA_Communications.h"
#include "Roles/Perception/PHY_RoleHLA_Perceiver.h"
#include "Roles/Hla/HLA_RoleDistantPion.h"
#include "Roles/Population/PHY_RoleHLA_Population.h"
#include <hla/HLA_Serialization.h>
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA constructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MIL_AgentHLA::MIL_AgentHLA( uint nID, const ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager )
    : MIL_Agent_ABC( "", nID )
    , pArmy_( 0 )
    , pType_( 0 )
    , bPc_( false )
{
    RegisterRole< HLA_RoleDistantPion        >( *this, objectId );
    RegisterRole< dotation::PHY_RoleHLA_Dotations      >( *this );
    RegisterRole< PHY_RoleHLA_Location       >( *this );
    RegisterRole< PHY_RoleHLA_Posture        >( *this );
    RegisterRole< PHY_RoleHLA_Composantes    >( *this, interactionManager );
    RegisterRole< PHY_RoleHLA_NBC            >();
    RegisterRole< PHY_RoleHLA_Communications >();
    RegisterRole< PHY_RoleHLA_Perceiver      >();
    RegisterRole< surrender::PHY_RoleHLA_Surrender      >();
    RegisterRole< PHY_RoleHLA_Refugee        >();
    RegisterRole< PHY_RoleHLA_Population     >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA destructor
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MIL_AgentHLA::~MIL_AgentHLA()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetArmy
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_AgentHLA::GetArmy() const
{
    if( pArmy_ )
        return *pArmy_;
    throw std::exception( "army not set" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetType
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC& MIL_AgentHLA::GetType() const
{
    if( pType_ )
        return *pType_;
    throw std::exception( "type not set" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::IsDead
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::IsDead() const
{
    return GetRole< PHY_RoleHLA_Composantes >().GetMajorComposante() == 0;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::IsNeutralized
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::IsNeutralized() const
{
    return GetRole< PHY_RoleHLA_Composantes >().IsNeutralized();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::IsPC
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::IsPC() const
{
    return bPc_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::NotifyAttackedBy
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
void MIL_AgentHLA::NotifyAttackedBy( MIL_AgentPion& )
{
    // $$$$ AGE 2004-12-10: interaction
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_AgentHLA::NotifyAttackedBy( MIL_Population& population )
{
    // $$$$ NLD/SBO 2005-12-01: interaction ?
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::CreateKnowledge
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > MIL_AgentHLA::CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup )
{
    boost::shared_ptr< DEC_Knowledge_Agent > result( new DEC_Knowledge_Agent( knowledgeGroup, *this ) );
    return result;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::IsPerceived
// Created: AGE 2004-11-09
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::IsPerceived( const MIL_Agent_ABC& /*agent*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::BelongsTo
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::BelongsTo( const MIL_KnowledgeGroup& /*group*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::Deserialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void MIL_AgentHLA::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    const bool bWasInvalid = ! IsValid();
    if( attributeID == "armee" )
    {
        std::string strArmy;
        deserializer >> strArmy;
        //pArmy_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( strArmy );//@TODO MGD give armies resolver ton HLA role
    }
    else if( attributeID == "type" )
    {
        std::string strType;
        deserializer >> strType;
        pType_ = MIL_AgentTypePion::Find( strType );
    }
    else if( attributeID == "status" )
    {
        std::vector< std::string > statuses;
        deserializer >> statuses;
        ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Composantes >   ().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Communications >().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_NBC >           ().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Posture >       ().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Perceiver >     ().ChangeStatus( statuses );
        GetRole< surrender::PHY_RoleHLA_Surrender >     ().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Refugee >       ().ChangeStatus( statuses );
        GetRole< PHY_RoleHLA_Population >    ().ChangeStatus( statuses );
    }
    if( bWasInvalid && IsValid() && GetRole< PHY_RoleHLA_Location >().IsValid() )
        GetRole< PHY_RoleHLA_Location >().UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::IsValid
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
bool MIL_AgentHLA::IsValid() const
{
    return pArmy_ && pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::ChangeStatus
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
void MIL_AgentHLA::ChangeStatus( const std::vector< std::string >& statuses )
{
    // $$$$ AGE 2004-12-13: Not really a status, more like a permanent attribute.
    bPc_ = std::find( statuses.begin(), statuses.end(), "pc" ) != statuses.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetDecision
// Created: LDC 2009-03-30
// -----------------------------------------------------------------------------
DEC_Decision_ABC& MIL_AgentHLA::GetDecision()
{
    throw std::runtime_error( __FUNCTION__ " not available for HLA Agents" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetDecision
// Created: LDC 2009-03-30
// -----------------------------------------------------------------------------
const DEC_Decision_ABC& MIL_AgentHLA::GetDecision() const
{
    throw std::runtime_error( __FUNCTION__ " not available for HLA Agents" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetKnowledge
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_AgentPion& MIL_AgentHLA::GetKnowledge() const
{
    throw std::runtime_error( __FUNCTION__ " not available for HLA Agents" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentHLA::GetKnowledge
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
const AlgorithmsFactories& MIL_AgentHLA::GetAlgorithms() const
{
    throw std::runtime_error( __FUNCTION__ " not available for HLA Agents" );
}
