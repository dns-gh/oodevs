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

#include "MIL_pch.h"

#include "MIL_NuageNBC.h"

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

BOOST_CLASS_EXPORT_GUID( MIL_NuageNBC, "MIL_NuageNBC" )

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_NuageNBC::MIL_NuageNBC()
    : MIL_RealObject_ABC( MIL_RealObjectType::nuageNBC_ )
    , pNbcAgentType_    ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_NuageNBC::~MIL_NuageNBC()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_NuageNBC::load( MIL_CheckPointInArchive& file , const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    
    uint nTypeID;
    file >> nTypeID;
    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( nTypeID );
    assert( pNbcAgentType_ );
    
    file >> nDeathTimeStep_
         >> vOrigin_
         >> rCurrentCircleRadius_
         >> rCurrentPropagationLenght_;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_NuageNBC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file << pNbcAgentType_->GetID()
         << nDeathTimeStep_
         << vOrigin_
         << rCurrentCircleRadius_
         << rCurrentPropagationLenght_;
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: JVT 02-10-22
// Modified: JVT 2004-10-28
//-----------------------------------------------------------------------------
bool MIL_NuageNBC::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void MIL_NuageNBC::Initialize( MIL_Army& army, const TER_Localisation& localisation, const MIL_NbcAgentType& nbcAgent )
{
    MIL_RealObject_ABC::Initialize( army, localisation );

    assert( localisation.WasACircle() );
    pNbcAgentType_        = &nbcAgent;

    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgentType_->GetLifeTime();    
}

//-----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2003-07-21
// Modified: JVT 2004-10-28
//-----------------------------------------------------------------------------
void MIL_NuageNBC::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    if( !GetLocalisation().WasACircle() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Localisation of object type 'NuageNBC' MUST be a circle", archive.GetContext() );

    std::string strNbcAgentType_;
    archive.Section( "AgentNBC" );
    archive.ReadAttribute( "type", strNbcAgentType_ );
    archive.EndSection();

    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( strNbcAgentType_ );
    if( !pNbcAgentType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Unknown 'AgentNBC' '%s' for NBC object '%d'", strNbcAgentType_.c_str(), GetID() ), archive.GetContext() );

    if( !GetLocalisation().WasACircle() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Localisation type of object 'nuage NBC' should be a circle", archive.GetContext() );    

    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgentType_->GetLifeTime();
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::Initialize
// Created: NLD 2003-08-04
// Modified: JVT 2004-10-28
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_NuageNBC::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( asnCreateObject.localisation.type != EnumTypeLocalisation::circle )
        return EnumObjectErrorCode::error_invalid_localisation;
   
    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_nuage_nbc )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( asnCreateObject.attributs_specifiques.u.nuage_nbc->agent_nbc );
    if( !pNbcAgentType_ )
        return EnumObjectErrorCode::error_invalid_specific_attributes;

    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    vOrigin_              = GetLocalisation().GetCircleCenter();
    rCurrentCircleRadius_ = GetLocalisation().GetCircleRadius();
    nDeathTimeStep_       = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + pNbcAgentType_->GetLifeTime();   
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
    MIL_RealObject_ABC::ProcessAgentInside( agent );

    if( CanInteractWithAgent( agent ) )
        agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgentType_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::ProcessAgentEntering
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_NuageNBC::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );

    if( CanInteractWithAgent( agent ) )
        agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgentType_ );
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
    assert( pNbcAgentType_ );
    asnAttributes_.agent_nbc  = pNbcAgentType_->GetID();

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent    = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t           = T_AttrObjectSpecific_nuage_nbc;
    asnMsg.GetAsnMsg().attributs_specifiques.u.nuage_nbc = &asnAttributes_;
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
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::UpdateShape
// Created: JVT 2004-10-29
// -----------------------------------------------------------------------------
bool MIL_NuageNBC::UpdateShape( TER_Localisation& newLocalisation )
{
    assert( pNbcAgentType_ );

    const PHY_Meteo::sWindData& wind = MIL_Tools::GetWind( vOrigin_ );

    if ( wind.rWindSpeed_ <= pNbcAgentType_->GetMinPropagationSpeed() )
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
        T_PointVector points;

        // demi cercle
        const MT_Float rInitialAngle = Angle( MT_Vector2D( 0., 1. ), wind.vWindDirection_ );
        for ( MT_Float rAngle = 0; rAngle <= MT_PI; rAngle += MT_PI / 8 )
            points.push_back( MT_Vector2D( vOrigin_.rX_ + rCurrentCircleRadius_ * cos( rInitialAngle - rAngle ), vOrigin_.rY_ + rCurrentCircleRadius_ * sin( rInitialAngle - rAngle ) ) );

        // + cône
        points.push_back( points.back()  + rCurrentPropagationLenght_ * wind.vWindDirection_.Rotated( -0.5 * pNbcAgentType_->GetPropagationAngle() ) );
        points.push_back( points.front() + rCurrentPropagationLenght_ * wind.vWindDirection_.Rotated(  0.5 * pNbcAgentType_->GetPropagationAngle() ) );

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
    
    if ( nDeathTimeStep_ <= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
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
bool MIL_NuageNBC::Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    MIL_RealObject_ABC::Initialize( strOption, strExtra, rCompletion, rMining, rBypass );
    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( strOption );
    nDeathTimeStep_ = uint( -1 );
    return pNbcAgentType_ != 0;
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
        const MIL_NbcAgentType* pNbcAgent = MIL_NbcAgentType::FindNbcAgentType( strNBC );
        if( pNbcAgent )
            pNbcAgentType_ = pNbcAgent;
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
    MIL_RealObject_ABC::Serialize( functor );
    functor.Serialize( "option", false, pNbcAgentType_->GetName() );
}
