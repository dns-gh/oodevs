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
#include "Entities/Specialisations/LOG/ObjectLogisticHierarchy.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "protocol/Protocol.h"
#include "Checkpoints/SerializationTools.h"
#include <xeumeuleu/xml.hpp>
#include <boost/assign/list_of.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyRecon< LogisticAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyRecon< LogisticAttribute > )

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis )
{
    unsigned int logSuperiorId;
    xis >> xml::attribute( "id", logSuperiorId );

    MIL_AutomateLOG* pLogSuperior = MIL_AgentServer::GetWorkspace().GetEntityManager().FindBrainLogistic( logSuperiorId );
    if( !pLogSuperior )
        xis.error( "Invalid logistic base" );
    pLogisticHierarchy_.reset( new logistic::ObjectLogisticHierarchy( *pLogSuperior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const sword::MissionParameter_Value& attributes )
{
    MIL_AutomateLOG* pLogSuperior = MIL_AgentServer::GetWorkspace().GetEntityManager().FindBrainLogistic( attributes.list( 1 ).identifier() );
    if( !pLogSuperior )
        throw std::runtime_error( "Invalid logistic base" );
    pLogisticHierarchy_.reset( new logistic::ObjectLogisticHierarchy( *pLogSuperior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::LogisticAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute()
    : pLogisticHierarchy_( new logistic::ObjectLogisticHierarchy() )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    pLogisticHierarchy_.reset();
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::WriteODB
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
void LogisticAttribute::WriteODB( xml::xostream& xos ) const
{
    MIL_AutomateLOG* pSuperior = pLogisticHierarchy_->GetPrimarySuperior();
    if( !pSuperior )
        return; // It sucks ...

    xos << xml::start( "logistic-base" )
            << xml::attribute( "id", pSuperior->GetID() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void LogisticAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void LogisticAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & pLogisticHierarchy_;
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
void LogisticAttribute::SendFullState( sword::ObjectAttributes& msg ) const
{
    MIL_AutomateLOG* pSuperior = pLogisticHierarchy_->GetPrimarySuperior();
    if( pSuperior )
        pSuperior->Serialize( *msg.mutable_logistic()->mutable_logistic_superior() );
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
    //$$$ POURRI TMP
    pLogisticHierarchy_->ChangeLinks( boost::assign::list_of( rhs.pLogisticHierarchy_->GetPrimarySuperior() ) );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool LogisticAttribute::Update( const LogisticAttribute& rhs )
{
    //$$$ POURRI TMP
    if( rhs.pLogisticHierarchy_->GetPrimarySuperior() != pLogisticHierarchy_->GetPrimarySuperior() )
    {
        pLogisticHierarchy_->ChangeLinks( boost::assign::list_of( rhs.pLogisticHierarchy_->GetPrimarySuperior() ) );
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::GetLogisticHierarchy
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
const logistic::LogisticHierarchy_ABC& LogisticAttribute::GetLogisticHierarchy() const
{
    return *pLogisticHierarchy_;
}
