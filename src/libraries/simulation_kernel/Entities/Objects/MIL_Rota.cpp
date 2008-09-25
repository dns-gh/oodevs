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
#include "Hla/HLA_UpdateFunctor.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include <hla/Deserializer.h>
#include <hla/AttributeIdentifier.h>
#include <xeumeuleu/xml.h>

using namespace hla;

BOOST_CLASS_EXPORT_GUID( MIL_Rota, "MIL_Rota" )

//-----------------------------------------------------------------------------
// Name: MIL_Rota constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Rota::MIL_Rota( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , nDanger_          ( 0 )
{
    asnAttributes_.nbc_agents.n = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Rota::MIL_Rota()
    : MIL_RealObject_ABC()
    , nDanger_          ( 0 )
{
    asnAttributes_.nbc_agents.n = 0;
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
        nbcAgents_.insert( MIL_NbcAgentType::Find( nID ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Rota::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    file << nDanger_;
    unsigned size = nbcAgents_.size();
    file << size;

    for ( CIT_NbcAgentSet it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it )
    {
        unsigned id = (*it)->GetID();
        file << id;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_Rota::WriteSpecificAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "specific-attributes" )
        << xml::content( "danger", nDanger_ )
            << xml::start( "nbc-agents" );

    for( CIT_NbcAgentSet it = nbcAgents_.begin(); it != nbcAgents_.end(); ++it )
    {
        xos << xml::start( "nbc-agent" )
            << xml::attribute( "type", (**it).GetName() )
            << xml::end(); // nbc-agent
    }
    xos        << xml::end()
        << xml::end();
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_Rota::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx );
    return false; // Creation des Rotas depuis les scripts interdite
}

//-----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_Rota::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    xis >> xml::start( "specific-attributes" )
            >> xml::content( "danger", nDanger_ );

    if( nDanger_ > 10 || nDanger_ < 0 )
        xis.error( "specific-attributes is not in bound( 0, 10 )" );
    
    nbcAgents_.clear();
    xis >> xml::start( "nbc-agents" )
            >> xml::list( "nbc-agent", *this, &MIL_Rota::ReadNbcAgent )
        >> xml::end();

    xis >> xml::end(); // specific-attributes
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::ReadNbcAgent
// Created: ABL 2007-07-09
// -----------------------------------------------------------------------------
void MIL_Rota::ReadNbcAgent( xml::xistream& xis )
{
    std::string strNbcTmp;
    xis >> xml::attribute( "type", strNbcTmp );

    const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( strNbcTmp );
    if( !pNbcAgentType )
        xis.error( "Unknown 'AgentNBC' '" + strNbcTmp + "' for rota object" );
    nbcAgents_.insert( pNbcAgentType );
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_Rota::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_rota )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    nDanger_ = asnCreateObject.specific_attributes.u.rota->danger_level;

    nbcAgents_.clear();
    for( uint i = 0; i < asnCreateObject.specific_attributes.u.rota->nbc_agents.n; ++i )
    {
        const MIL_NbcAgentType* pNbcAgentType = MIL_NbcAgentType::Find( asnCreateObject.specific_attributes.u.rota->nbc_agents.elem[i] );
        if( !pNbcAgentType )
            return EnumObjectErrorCode::error_invalid_specific_attributes;
        nbcAgents_.insert( pNbcAgentType );
    }

    return MIL_RealObject_ABC::Initialize( asnCreateObject );
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
    asnAttributes_.danger_level = nDanger_;
    asnAttributes_.nbc_agents.n = nbcAgents_.size();
    if( !nbcAgents_.empty() )
    {
        asnAttributes_.nbc_agents.elem = new ASN1T_OID[ nbcAgents_.size() ];
        uint i = 0;
        for( CIT_NbcAgentSet itNbc = nbcAgents_.begin(); itNbc != nbcAgents_.end(); ++itNbc, ++i )
            asnAttributes_.nbc_agents.elem[ i ]  = (**itNbc).GetID();
    }

    asnMsg().m.specific_attributesPresent = 1;
    asnMsg().specific_attributes.t        = T_ObjectAttributesSpecific_rota;
    asnMsg().specific_attributes.u.rota   = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::CleanSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void MIL_Rota::CleanSpecificAttributes()
{
    if( asnAttributes_.nbc_agents.n > 0 )
        delete [] asnAttributes_.nbc_agents.elem;
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
bool MIL_Rota::Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass )
{
    return     MIL_RealObject_ABC::Initialize( localisation, strOption, strExtra, rCompletion, rMining, rBypass )
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
        const MIL_NbcAgentType* pNbcAgent = MIL_NbcAgentType::Find( strAgent );
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
