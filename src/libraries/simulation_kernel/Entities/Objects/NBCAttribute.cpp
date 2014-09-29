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
#include "Object.h"
#include "MIL_NbcAgentType.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( NBCAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyRecon< NBCAttribute > )

// -----------------------------------------------------------------------------
// Name: NBCAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
NBCAttribute::NBCAttribute( xml::xistream& xis )
    : nForm_ ( eGas )
    , danger_( xis.attribute< int >( "danger", 0 ) )
{
    std::string state( "gaseous" );
    xis >> xml::optional
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
NBCAttribute::NBCAttribute( const sword::MissionParameter_Value& attributes )
    : nForm_ ( eGas )
    , danger_( attributes.list( 1 ).quantity() )
{
    const sword::MissionParameter_Value& agentsList = attributes.list( 2 );
    for( int i = 0; i < agentsList.list_size(); ++i )
    {
        const MIL_NbcAgentType* pType = MIL_NbcAgentType::Find( agentsList.list( i ).identifier() );
        if( !pType )
            throw MASA_EXCEPTION( "Unknown agent type for NBC attribute" );
        agents_.push_back( pType );
    }
    if( attributes.list_size() == 4 )
        nForm_ = static_cast< E_Form >( attributes.list( 3 ).quantity() );
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
// Name: NBCAttribute::ReadNBCAgent
// Created: JCR 2008-08-27
// -----------------------------------------------------------------------------
void NBCAttribute::ReadNBCAgent( xml::xistream& xis )
{
    const std::string type = xis.attribute< std::string >( "type", std::string() );
    const MIL_NbcAgentType* pType = MIL_NbcAgentType::Find( type );
    if( ! pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown 'AgentNBC' '" + type + "' for NBC object" );
    agents_.push_back( pType );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void NBCAttribute::load( MIL_CheckPointInArchive& file , const unsigned int )
{
    file >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file >> danger_;
    file >> nForm_;
    file >> agents_;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void NBCAttribute::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file << danger_;
    file << nForm_;
    file << agents_;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void NBCAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< NBCAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyRecon< NBCAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void NBCAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< NBCAttribute, NBCAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void NBCAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_nbc()->set_danger_level( danger_ );
    asn.mutable_nbc()->mutable_nbc_agents();
    for( auto it = agents_.begin(); it != agents_.end(); ++it )
        asn.mutable_nbc()->add_nbc_agents()->set_id( (*it)->GetID() );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool NBCAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
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
    for( auto it = agents_.begin(); it != agents_.end(); ++it )
        xos << xml::start( "nbc-agent" ) << xml::attribute( "type", (*it)->GetName() )
            << xml::end;
    xos << xml::end;
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
    if( agents_.empty() )
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
    if( agents_.empty() )
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
// Name: NBCAttribute::GetDangerLevel
// Created: JSR 2010-05-21
// -----------------------------------------------------------------------------
unsigned int NBCAttribute::GetDangerLevel() const
{
    return danger_;
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
        const MIL_NbcAgentType* type = MIL_NbcAgentType::Find( strAgent );
        if( ! type )
            return false;
        agents_.push_back( type );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::WriteAgents
// Created: AGE 2004-12-01
// -----------------------------------------------------------------------------
std::string NBCAttribute::WriteAgents() const
{
    std::string strAgents;
    for( auto it = agents_.begin(); it != agents_.end(); ++it )
        strAgents += (*it)->GetName() + '\n';
    return strAgents;
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool NBCAttribute::Update( const NBCAttribute& rhs )
{
    if( agents_ != rhs.agents_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        agents_ = rhs.agents_;
    }
    if( nForm_ != rhs.nForm_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        nForm_ = rhs.nForm_;
    }
    if( danger_ != rhs.danger_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        danger_ = rhs.danger_;
    }
    return NeedUpdate( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: NBCAttribute::UpdateCloudAttribute
// Created: LGY 2012-01-13
// -----------------------------------------------------------------------------
bool NBCAttribute::UpdateCloudAttribute( const NBCAttribute& rhs )
{
    agents_.clear();
    for( auto it = rhs.agents_.begin(); it != rhs.agents_.end(); ++it )
        if( (*it)->IsGasContaminating() || (*it)->IsGasPoisonous() )
            agents_.push_back( *it );
    if( !agents_.empty() )
        NotifyAttributeUpdated( eOnUpdate );
    if( danger_ != rhs.danger_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        danger_ = rhs.danger_;
    }
    nForm_ = eGas;
    return NeedUpdate( eOnUpdate );
}
