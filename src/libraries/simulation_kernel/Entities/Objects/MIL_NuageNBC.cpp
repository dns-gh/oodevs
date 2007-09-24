//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NuageNBC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:36 $
// $Revision: 11 $
// $Workfile: MIL_NuageNBC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_NuageNBC.h"

#include "MIL_NbcAgent.h"
#include "MIL_NbcAgentType.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectNuageNBC.h"
#include "Network/NET_ASN_Messages.h"

#include "Tools/MIL_Tools.h"

#include "HLA/AttributeIdentifier.h"
#include "HLA/Deserializer.h"
#include "HLA/HLA_UpdateFunctor.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

#include "xeumeuleu/xml.h"

using namespace xml;

BOOST_CLASS_EXPORT_GUID( MIL_NuageNBC, "MIL_NuageNBC" )

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_NuageNBC::MIL_NuageNBC( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , pNbcAgent_        ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_NuageNBC::MIL_NuageNBC()
    : MIL_RealObject_ABC()
    , pNbcAgent_        ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_NuageNBC::~MIL_NuageNBC()
{
    if( pNbcAgent_ )
        delete pNbcAgent_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_NuageNBC::serialize
// Created: NLD 2006-10-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void MIL_NuageNBC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file & pNbcAgent_
         & nDeathTimeStep_
         & vOrigin_
         & rCurrentCircleRadius_
         & rCurrentPropagationLenght_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_NuageNBC::WriteSpecificAttributes( xml::xostream& xos ) const
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

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: JVT 02-10-22
// Modified: JVT 2004-10-28
//-----------------------------------------------------------------------------
bool MIL_NuageNBC::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void MIL_NuageNBC::Initialize( const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgentType )
{
    MIL_RealObject_ABC::Initialize( localisation );

    assert( localisation.WasACircle() );
    pNbcAgent_            = new MIL_NbcAgent( nbcAgentType, MIL_NbcAgent::eGas );
    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgent_->GetLifeTime();
}

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2003-07-21
// Modified: JVT 2004-10-28
//-----------------------------------------------------------------------------
void MIL_NuageNBC::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    if( !GetLocalisation().WasACircle() )
        xis.error( "Localisation of object type 'NuageNBC' MUST be a circle" );

    xis >> start( "specific-attributes" )
            >> start( "nbc-agent" );

    std::string strNbcAgentType_;
    xis >> attribute( "type", strNbcAgentType_ );
    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strNbcAgentType_ );
    if( !pNbcAgentType )
        xis.error( "Unknown 'AgentNBC' '" + strNbcAgentType_ + "'" );
    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eGas );

    xis >> end()
        >> end();

    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgent_->GetLifeTime();
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2003-08-04
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_NuageNBC::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( asnCreateObject.location.type != EnumLocationType::circle )
        return EnumObjectErrorCode::error_invalid_localisation;
   
    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_nbc_cloud )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( asnCreateObject.specific_attributes.u.nbc_cloud->nbc_agent );
    if( !pNbcAgentType )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eGas );

    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgent_->GetLifeTime();   
    return EnumObjectErrorCode::no_error;
}

//=============================================================================
// EVENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::ProcessAgentInside
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NuageNBC::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( CanInteractWith( agent ) )
    {
        if( pNbcAgent_->IsContaminating() )
            agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgent_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::ProcessAgentEntering
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NuageNBC::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    if( CanInteractWith( agent ) )
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
// Name: MIL_NuageNBC::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_NuageNBC::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pNbcAgent_ );
    asnAttributes_.nbc_agent  = pNbcAgent_->GetType().GetID();

    asnMsg().m.specific_attributesPresent    = 1;
    asnMsg().specific_attributes.t           = T_ObjectAttributesSpecific_nbc_cloud;
    asnMsg().specific_attributes.u.nbc_cloud = &asnAttributes_;
}


// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_NuageNBC::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectNuageNBC( teamKnowing, *this );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::GetNbcAgentType
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
const MIL_NbcAgentType& MIL_NuageNBC::GetNbcAgentType() const
{
    assert( pNbcAgent_ );
    return pNbcAgent_->GetType();
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::UpdateShape
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
bool MIL_NuageNBC::UpdateShape( TER_Localisation& newLocalisation )
{
    assert( pNbcAgent_ );

    const PHY_Meteo::sWindData& wind = MIL_Tools::GetWind( vOrigin_ );

    if ( wind.rWindSpeed_ <= MIL_NbcAgentType::GetMinPropagationSpeed() )
    {
        if ( wind.rWindSpeed_ <= 0. )
            return false;

        // propagation en en cercle
        rCurrentCircleRadius_ += wind.rWindSpeed_;
    }
    else
        // Propagation dirigée
        rCurrentPropagationLenght_ = std::max( rCurrentPropagationLenght_, rCurrentCircleRadius_ ) + wind.rWindSpeed_;
    
    // Création de la localisation
    if ( rCurrentCircleRadius_ >= rCurrentPropagationLenght_ )
    {
        // cercle
        T_PointVector points;
        points.push_back( vOrigin_ );
        points.push_back( vOrigin_ + MT_Vector2D( 0., rCurrentCircleRadius_ ) );

        newLocalisation.Reset( TER_Localisation::eCircle, points );    
    }
    else
    {
        T_PointVector points; points.reserve( 12 );

        // demi cercle
        const MT_Float rInitialAngle = Angle( MT_Vector2D( 0., 1. ), wind.vWindDirection_ );
        for ( MT_Float rAngle = 0; rAngle <= MT_PI; rAngle += MT_PI / 8 )
            points.push_back( MT_Vector2D( vOrigin_.rX_ + rCurrentCircleRadius_ * cos( rInitialAngle - rAngle ), vOrigin_.rY_ + rCurrentCircleRadius_ * sin( rInitialAngle - rAngle ) ) );

        // + cône
        points.push_back( points.back()  + rCurrentPropagationLenght_ * wind.vWindDirection_.Rotated( -0.5 * pNbcAgent_->GetPropagationAngle() ) );
        points.push_back( points.front() + rCurrentPropagationLenght_ * wind.vWindDirection_.Rotated(  0.5 * pNbcAgent_->GetPropagationAngle() ) );

        newLocalisation.Reset( TER_Localisation::ePolygon, points );
    }    
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::UpdateState
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void MIL_NuageNBC::UpdateState()
{
    MIL_RealObject_ABC::UpdateState();
    
    if( nDeathTimeStep_ <= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    {
        MarkForDestruction();
        return;
    }
    
    TER_Localisation newLocalisation;
    if( UpdateShape( newLocalisation ) )
        UpdateLocalisation( newLocalisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_NuageNBC::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    nDeathTimeStep_ = uint( -1 );
    MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass );
    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strOption );
    if( !pNbcAgentType )
        return false;
    pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eGas );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Deserialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_NuageNBC::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strNBC;
        deserializer >> strNBC;
        const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strNBC );
        if( pNbcAgentType )
            pNbcAgent_ = new MIL_NbcAgent( *pNbcAgentType, MIL_NbcAgent::eGas );
    }
    else
        MIL_RealObject_ABC::Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Serialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_NuageNBC::Serialize( HLA_UpdateFunctor& functor ) const
{
    assert( pNbcAgent_ );
    MIL_RealObject_ABC::Serialize( functor );
    functor.Serialize( "option", false, pNbcAgent_->GetType().GetName() );
}
