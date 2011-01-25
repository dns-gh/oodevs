// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LogisticAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyRecon< LogisticAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyRecon< LogisticAttribute > )

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis )
{
    unsigned int nTC2;
    xis >> xml::attribute( "id", nTC2 );

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2 );
    if( !pTC2Tmp )
        xis.error( "Automate TC2 specified is invalid" );
    if( !pTC2Tmp->GetType().IsLogistic() )
        xis.error( "Automate TC2 specified is not a logistic automate" );
    pTC2_ = pTC2Tmp->GetBrainLogistic();
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const sword::MissionParameter_Value& attributes )
{
    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( attributes.list( 1 ).identifier() );
    if( !pTC2Tmp )
        throw std::runtime_error( "Automate TC2 specified is invalid" );
    if( !pTC2Tmp->GetType().IsLogistic() )
        throw std::runtime_error( "Automate TC2 specified is not a logistic automate" );
    pTC2_ = pTC2Tmp->GetBrainLogistic();
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::LogisticAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute()
    : pTC2_ ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::WriteODB
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
void LogisticAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "tc2" )
            << xml::attribute( "id", pTC2_->GetID() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void LogisticAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void LogisticAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & pTC2_;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void LogisticAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< LogisticAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void LogisticAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< LogisticAttribute, LogisticAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void LogisticAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_logistic()->mutable_combat_train()->set_id( pTC2_->GetID() );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void LogisticAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: DotationAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
LogisticAttribute& LogisticAttribute::operator=( const LogisticAttribute& rhs )
{
    pTC2_ = rhs.pTC2_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool LogisticAttribute::Update( const LogisticAttribute& rhs )
{
    if( pTC2_ != rhs.pTC2_ )
    {        
        pTC2_ = rhs.pTC2_;
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::GetTC2
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_AutomateLOG& LogisticAttribute::GetTC2() const
{
    return *pTC2_;
}
