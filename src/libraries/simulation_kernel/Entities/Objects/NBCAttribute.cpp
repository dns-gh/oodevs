// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NBCAttribute.h"
#include "MIL.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeNBC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "hla/HLA_UpdateFunctor.h"
#include "MIL_NbcAgentType.h"

#include <hla/Deserializer.h>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( NBCAttribute, "NBCAttribute" )

using namespace hla;

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( xml::xistream& xis )
    : nForm_ ( eGas )
    , danger_( xml::attribute( xis, "danger", 0 ) )
{   
    std::string state( "gaseous" );
    xis >> xml::optional() 
        >> xml::attribute( "state", state );
    if( state == "liquid" )
        nForm_ = eLiquid;
    xis >> xml::list( "nbc-agent", *this, &NBCAttribute::ReadNBCAgent );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute()
    : nForm_ ( eGas )
    , danger_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( const ASN1T_ObjectAttributes& asn )
    : nForm_ ( eGas )
    , danger_( asn.nbc.danger_level )
{
    //NOTHING
    for ( unsigned i = 0; i < asn.nbc.nbc_agents.n; ++i )
    {                
        const MIL_NbcAgentType* pType = MIL_NbcAgentType::Find( asn.nbc.nbc_agents.elem[ i ] );
        if( !pType )
            throw std::runtime_error( "Unknown agent type for NBC attribute" );
        agents_.push_back( pType );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
NBCAttribute::~NBCAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Insert
// Created: JCR 2008-09-05
// -----------------------------------------------------------------------------
template< typename T > 
bool NBCAttribute::Insert( const T& type )
{
    const MIL_NbcAgentType* pType = MIL_NbcAgentType::Find( type );
    if( pType )        
        agents_.push_back( pType );
    return pType != 0;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::ReadNBCAgent
// Created: JCR 2008-08-27
// -----------------------------------------------------------------------------
void NBCAttribute::ReadNBCAgent( xml::xistream& xis )
{
    std::string type( xml::attribute( xis, "type", std::string() ) );
    if ( ! Insert( type ) )
        xis.error( "Unknown 'AgentNBC' '" + type + "' for NBC object" );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void NBCAttribute::load( MIL_CheckPointInArchive& file , const uint )
{
    file >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
         
    uint nNbrNbcAgents;
    file >> danger_;
    file >> nNbrNbcAgents;
    while ( nNbrNbcAgents-- )
    {
        uint nID;
        file >> nID;
        if ( !Insert( nID ) )
            throw std::runtime_error( "Unknown 'AgentNBC' '" + boost::lexical_cast<std::string>( nID ) + "' for NBC object" );
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void NBCAttribute::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< ObjectAttribute_ABC >( *this );    
    
    file << danger_;
    unsigned size = agents_.size();
    file << size;
    for ( CIT_NBCAgents it = agents_.begin(); it != agents_.end(); ++it )
    {
        unsigned id = (*it)->GetID();
        file << id;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void NBCAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeNBC( *this ) );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void NBCAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.nbcPresent = 1;
    asn.nbc.nbc_agents.n = agents_.size();
    if ( asn.nbc.nbc_agents.n > 0 )
    {
        asn.nbc.nbc_agents.elem = new ASN1T_OID[ asn.nbc.nbc_agents.n ];
        int i = 0;
        for ( CIT_NBCAgents it = agents_.begin(); it != agents_.end(); ++it )
            asn.nbc.nbc_agents.elem[ i++ ] = (*it)->GetID();
    }
    asn.nbc.danger_level = danger_;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void NBCAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void NBCAttribute::WriteODB( xml::xostream& xos ) const
{    
    xos << xml::start( "nbc-agents" );
    if( nForm_ == eLiquid )
        xos << xml::attribute( "state", "liquid" );
    else
        xos << xml::attribute( "state", "gaseous" );
    xos << xml::attribute( "danger", danger_ );
    for ( CIT_NBCAgents it = agents_.begin(); it != agents_.end(); ++it )
        xos << xml::start( "nbc-agent" ) << xml::attribute( "type", (*it)->GetName() )
            << xml::end();
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
NBCAttribute& NBCAttribute::operator=( const NBCAttribute& rhs )
{
    nForm_ = rhs.nForm_;
    agents_ = rhs.agents_;
    danger_ = rhs.danger_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::IsContaminating
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
bool NBCAttribute::IsContaminating() const
{
    if ( agents_.size() == 0 )
        return false;
    if( nForm_ == eGas )
        return agents_.front()->IsGasContaminating();
    return agents_.front()->IsLiquidContaminating();
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::IsPoisonning
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
bool NBCAttribute::IsPoisonous() const
{
    if ( agents_.size() == 0 )
        return false;
    if( nForm_ == eGas )
        return agents_.front()->IsGasPoisonous();    
    return agents_.front()->IsLiquidPoisonous();
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::GetNBCAgents
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
const NBCAttribute::T_NBCAgents& NBCAttribute::GetNBCAgents() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Deserialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void NBCAttribute::Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "option" )
    {
        std::string strOption;
        deserializer >> strOption;
        ReadAgents( strOption );
    }    
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::ReadAgents
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
bool NBCAttribute::ReadAgents( const std::string& strAgents )
{
    std::istringstream stream( strAgents );
    std::string strAgent;
    while( std::getline( stream, strAgent ) )
    {
        if ( !Insert( strAgent ) )
            return false;
    }    
    return true;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Serialize
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
void NBCAttribute::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "option", false, WriteAgents() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::WriteAgents
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
std::string NBCAttribute::WriteAgents() const
{
    std::string strAgents;
    for( CIT_NBCAgents it = agents_.begin(); it != agents_.end(); ++it )
        strAgents += (*it)->GetName() + '\n';
    return strAgents;
}
