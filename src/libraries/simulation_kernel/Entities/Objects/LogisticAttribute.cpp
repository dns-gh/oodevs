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
#include "CheckPoints/SerializationTools.h"
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< LogisticAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< LogisticAttribute > )

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis )
    : idFromXML_( xis.attribute< unsigned int >( "id" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: RPD 2009-10-20
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const sword::MissionParameter_Value& attributes )
    : idFromXML_( 0 )
{
    MIL_AutomateLOG* pLogSuperior = MIL_AgentServer::GetWorkspace().GetEntityManager().FindBrainLogistic( attributes.list( 1 ).identifier() );
    if( !pLogSuperior )
        throw MASA_EXCEPTION( "Invalid logistic base" );
    pLogisticHierarchy_.reset( new logistic::ObjectLogisticHierarchy( *pLogSuperior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::LogisticAttribute
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute()
    : pLogisticHierarchy_( new logistic::ObjectLogisticHierarchy() )
    , idFromXML_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: LDC 2012-04-03
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( MIL_AutomateLOG& superior )
    : idFromXML_( 0 )
{
    pLogisticHierarchy_.reset( new logistic::ObjectLogisticHierarchy( superior ) );
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
            << xml::attribute( "id", pSuperior->GetLogisticId() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void LogisticAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void LogisticAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & pLogisticHierarchy_;
    file & idFromXML_;
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
bool LogisticAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: DotationAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
LogisticAttribute& LogisticAttribute::operator=( const LogisticAttribute& rhs )
{
    //$$$ POURRI TMP
    if( pLogisticHierarchy_.get() && rhs.pLogisticHierarchy_.get() )
        pLogisticHierarchy_->ChangeLinks( boost::assign::list_of( rhs.pLogisticHierarchy_->GetPrimarySuperior() ) );
    idFromXML_ = rhs.idFromXML_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Update
// Created: NLD 2010-10-26
// -----------------------------------------------------------------------------
bool LogisticAttribute::Update( const LogisticAttribute& rhs )
{
    //$$$ POURRI TMP
    idFromXML_ = rhs.idFromXML_;
    if( pLogisticHierarchy_.get() && rhs.pLogisticHierarchy_.get() && rhs.pLogisticHierarchy_->GetPrimarySuperior() != pLogisticHierarchy_->GetPrimarySuperior() )
    {
        pLogisticHierarchy_->ChangeLinks( boost::assign::list_of( rhs.pLogisticHierarchy_->GetPrimarySuperior() ) );
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Finalize
// Created: JSR 2011-11-04
// -----------------------------------------------------------------------------
void LogisticAttribute::Finalize()
{
    if( !pLogisticHierarchy_.get() || !pLogisticHierarchy_->HasSuperior() )
    {
        MIL_AutomateLOG* pLogSuperior = MIL_AgentServer::GetWorkspace().GetEntityManager().FindBrainLogistic( idFromXML_ );
        if( !pLogSuperior )
            throw MASA_EXCEPTION( "Invalid logistic base for logistic attribute : id = " + boost::lexical_cast< std::string >( idFromXML_ ) );
        pLogisticHierarchy_.reset( new logistic::ObjectLogisticHierarchy( *pLogSuperior ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::GetLogisticHierarchy
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
const logistic::LogisticHierarchy_ABC& LogisticAttribute::GetLogisticHierarchy() const
{
    return *pLogisticHierarchy_;
}
