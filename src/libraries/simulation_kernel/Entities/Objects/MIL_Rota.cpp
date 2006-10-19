//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Rota.cpp $
// $Author: Nld $
// $Modtime: 26/04/05 14:18 $
// $Revision: 9 $
// $Workfile: MIL_Rota.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Rota.h"

#include "MIL_NbcAgentType.h"
#include "MIL_RealObjectType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectRota.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

#include "Tools/MIL_Tools.h"
#include "hla/Deserializer.h"
#include "hla/AttributeIdentifier.h"
#include "Hla/HLA_UpdateFunctor.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( MIL_Rota, "MIL_Rota" )

//-----------------------------------------------------------------------------
// Name: MIL_Rota constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Rota::MIL_Rota()
    : MIL_RealObject_ABC( MIL_RealObjectType::rota_ )
    , nDanger_          ( 0 )
{
    asnAttributes_.agents_nbc.n = 0;
}

//-----------------------------------------------------------------------------
// Name: MIL_Rota destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Rota::~MIL_Rota()
{
    CleanSpecificAttributes();
}


// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Rota::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Rota::load( MIL_CheckPointInArchive& file , const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file >> nDanger_;
         
    uint nNbrNbcAgents;
    file >> nNbrNbcAgents;
    while ( nNbrNbcAgents-- )
    {
        uint nID;
        file >> nID;
        nbcAgents_.insert( MIL_NbcAgentType::FindNbcAgentType( nID ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Rota::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file << nDanger_
         << nbcAgents_.size();

    for ( CIT_NbcAgentSet it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it )
        file << (*it)->GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Rota::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteField( "Danger", nDanger_  );
    archive.Section( "AgentsNBC" );
    for( CIT_NbcAgentSet it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it )
    {
        archive.Section( "AgentNBC" );
        archive.WriteAttribute( "type", (**it).GetName() );
        archive.EndSection(); // AgentNBC
    }
    archive.EndSection(); // AgentsNBC
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_Rota::Initialize( const MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx );
    return false; // Creation des Rotas depuis les scripts interdite
}

//-----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_Rota::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    archive.ReadField( "Danger", nDanger_, CheckValueBound( 0, 10 ) );

    nbcAgents_.clear();
    archive.BeginList( "AgentsNBC" );
    while( archive.NextListElement() )
    {
        std::string strNbcTmp;
        archive.Section( "AgentNBC" );
        archive.ReadAttribute( "type", strNbcTmp );
        const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::FindNbcAgentType( strNbcTmp );
        if( !pNbcAgentType )
            throw MT_ScipioException( "MIL_Rota::Initialize", __FILE__, __LINE__, MT_FormatString( "Unknown 'AgentNBC' '%s' for rota object '%d'", strNbcTmp.c_str(), GetID() ), archive.GetContext() );
        nbcAgents_.insert( pNbcAgentType );
        archive.EndSection(); // AgentNBC
    }
    archive.EndList(); // AgentsNBC
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_Rota::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_rota )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    nDanger_ = asnCreateObject.attributs_specifiques.u.rota->niveau_danger;

    nbcAgents_.clear();
    for( uint i = 0; i < asnCreateObject.attributs_specifiques.u.rota->agents_nbc.n; ++i )
    {
        const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::FindNbcAgentType( asnCreateObject.attributs_specifiques.u.rota->agents_nbc.elem[i] );
        if( !pNbcAgentType )
            return EnumObjectErrorCode::error_invalid_specific_attributes;
        nbcAgents_.insert( pNbcAgentType );
    }

    return MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
}
   
// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Rota::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_Rota::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    CleanSpecificAttributes();
    asnAttributes_.niveau_danger = nDanger_;
    asnAttributes_.agents_nbc.n = nbcAgents_.size();
    if( !nbcAgents_.empty() )
    {
        asnAttributes_.agents_nbc.elem = new ASN1T_OID[ nbcAgents_.size() ];
        uint i = 0;
        for( CIT_NbcAgentSet itNbc = nbcAgents_.begin(); itNbc != nbcAgents_.end(); ++itNbc, ++i )
            asnAttributes_.agents_nbc.elem[ i ]  = (**itNbc).GetID();
    }

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t        = T_AttrObjectSpecific_rota;
    asnMsg.GetAsnMsg().attributs_specifiques.u.rota   = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::CleanSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void MIL_Rota::CleanSpecificAttributes()
{
    if( asnAttributes_.agents_nbc.n > 0 )
        delete [] asnAttributes_.agents_nbc.elem;
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Rota::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_Rota::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectRota( teamKnowing, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_Rota::Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    return MIL_RealObject_ABC::Initialize( strOption, strExtra, rCompletion, rMining, rBypass )
        && ReadAgents( strOption );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_Rota::Deserialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_Rota::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strOption;
        deserializer >> strOption;
        ReadAgents( strOption );
    }
    else
        MIL_RealObject_ABC::Deserialize( attributeID, deserializer );
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::Serialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void MIL_Rota::Serialize( HLA_UpdateFunctor& functor ) const
{
    MIL_RealObject_ABC::Serialize( functor );
    functor.Serialize( "option", false, WriteAgents() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::ReadAgents
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool MIL_Rota::ReadAgents( const std::string& strAgents )
{
    std::istringstream stream( strAgents );
    std::string strAgent;
    while( std::getline( stream, strAgent ) )
    {
        const MIL_NbcAgentType* pNbcAgent = MIL_NbcAgentType::FindNbcAgentType( strAgent );
        if( pNbcAgent )
            nbcAgents_.insert( pNbcAgent );
        else
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::WriteAgents
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
std::string MIL_Rota::WriteAgents() const
{
    std::string strAgents;
    for( CIT_NbcAgentSet it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it )
        strAgents += (*it)->GetName() + '\n';
    return strAgents;
}
