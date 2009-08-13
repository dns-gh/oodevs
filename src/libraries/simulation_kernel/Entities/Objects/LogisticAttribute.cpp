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
#include "Knowledge/DEC_Knowledge_ObjectAttributeLogistic.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( LogisticAttribute, "LogisticAttribute" )

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( xml::xistream& xis )    
{
    uint nTC2;   
    xis >> xml::attribute( "id", nTC2 );
            
    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2 );
    if( !pTC2Tmp )
        xis.error( "Automate TC2 specified is invalid" );
    if( !pTC2Tmp->GetType().IsLogistic() )
        xis.error( "Automate TC2 specified is not a logistic automate" );
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );
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
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void LogisticAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void LogisticAttribute::serialize( Archive& file, const uint )
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
    object.Attach( *new DEC_Knowledge_ObjectAttributeLogistic( *this ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void LogisticAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.logisticPresent = 1;
    asn.logistic.tc2 = pTC2_->GetID();    
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
/*caller*/void LogisticAttribute::SendUpdate( ASN1T_ObjectAttributes& /*asn*/ ) const
{
    // NOTHING
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
// Name: LogisticAttribute::GetTC2
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_AutomateLOG& LogisticAttribute::GetTC2() const
{
    return *pTC2_;
}
