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

#include "MIL_Pch.h"

#include "MIL_ZoneNBC.h"

#include "MIL_NbcAgentType.h"
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

BOOST_CLASS_EXPORT_GUID( MIL_ZoneNBC, "MIL_ZoneNBC" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneNBC::MIL_ZoneNBC()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneNBC_ )
    , pNbcAgentType_    ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneNBC::~MIL_ZoneNBC()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::load( MIL_CheckPointInArchive& file , const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    
    uint nID;
    file >> nID;
    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( nID );
    assert( pNbcAgentType_ );

    file >> decontaminatedZones_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file << pNbcAgentType_->GetID()
         << decontaminatedZones_;
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_ZoneNBC::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx );
    return false;
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_ZoneNBC::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    if( !GetLocalisation().WasACircle() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Localisation of object type 'ZoneNBC' MUST be a circle", archive.GetContext() );

    std::string strNbcAgentType_;
    archive.Section( "AgentNBC" );
    archive.ReadAttribute( "type", strNbcAgentType_ );
    archive.EndSection();
    
    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( strNbcAgentType_ );
    if( !pNbcAgentType_ )
        throw MT_ScipioException( "MIL_ZoneNBC::Initialize", __FILE__, __LINE__, MT_FormatString( "Unknown 'AgentNBC' '%s' for NBC object '%d'", strNbcAgentType_.c_str(), GetID() ), archive.GetContext() );

    MIL_NuageNBC& nuage = *new MIL_NuageNBC();
    nuage.Initialize( GetArmy(), GetLocalisation(), *pNbcAgentType_ );
    MIL_AgentServer::GetWorkspace().GetEntityManager().RegisterObject( nuage );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ZoneNBC::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( asnCreateObject.localisation.type != EnumTypeLocalisation::circle )
        return EnumObjectErrorCode::error_invalid_localisation;

    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_zone_nbc )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( asnCreateObject.attributs_specifiques.u.zone_nbc->agent_nbc );
    if( !pNbcAgentType_ )
        return EnumObjectErrorCode::error_invalid_specific_attributes;

    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    MIL_NuageNBC& nuage = *new MIL_NuageNBC();
    nuage.Initialize( GetArmy(), GetLocalisation(), *pNbcAgentType_ );
    MIL_AgentServer::GetWorkspace().GetEntityManager().RegisterObject( nuage );

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
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( !IsAgentInsideDecontaminatedZone( agent ) )
        agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgentType_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::ProcessAgentEntering
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_ZoneNBC::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    if( !IsAgentInsideDecontaminatedZone( agent ) )
        agent.GetRole< PHY_RoleInterface_NBC >().Contaminate( *pNbcAgentType_ );
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
    assert( pNbcAgentType_ );
    asnAttributes_.agent_nbc  = pNbcAgentType_->GetID();

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent   = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t          = T_AttrObjectSpecific_zone_nbc;
    asnMsg.GetAsnMsg().attributs_specifiques.u.zone_nbc = &asnAttributes_;
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


// -----------------------------------------------------------------------------
// Name: MIL_ZoneNBC::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_ZoneNBC::Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    MIL_RealObject_ABC::Initialize( strOption, strExtra, rCompletion, rMining, rBypass );
    pNbcAgentType_ = MIL_NbcAgentType::FindNbcAgentType( strOption );
    return pNbcAgentType_ != 0;
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
        const MIL_NbcAgentType* pNbcAgent = MIL_NbcAgentType::FindNbcAgentType( strNBC );
        if( pNbcAgent )
            pNbcAgentType_ = pNbcAgent;
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
    MIL_RealObject_ABC::Serialize( functor );
    functor.Serialize( "option", false, pNbcAgentType_->GetName() );
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