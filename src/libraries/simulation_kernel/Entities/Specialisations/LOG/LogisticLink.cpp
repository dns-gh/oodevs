// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LogisticLink.h"
#include "MIL_AutomateLOG.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/NET_AsnException.h"
#include "protocol/ClientSenders.h"
#pragma warning( push, 0 )
#include <boost/range/algorithm.hpp>
#pragma warning( pop )

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::LogisticLink )

// -----------------------------------------------------------------------------
// Name: LogisticLink constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
LogisticLink::LogisticLink( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& superior, bool useQuotas, xml::xistream& xis )
    : owner_        ( &owner )
    , superior_     ( &superior )
    , useQuotas_    ( useQuotas )
    , quotasUpdated_( useQuotas )
{
    if( useQuotas_ )
    {
        xis >> xml::optional >> xml::start( "quotas" )
                >> xml::list( "resource", *this, &LogisticLink::ReadQuota )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLink constructor
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
LogisticLink::LogisticLink( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& superior, bool useQuotas )
    : owner_        ( &owner )
    , superior_     ( &superior )
    , useQuotas_    ( useQuotas )
    , quotasUpdated_( useQuotas )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink constructor
// Created: LDC 2013-01-17
// -----------------------------------------------------------------------------
LogisticLink::LogisticLink()
    : owner_        ( 0 )
    , superior_     ( 0 )
    , useQuotas_    ( false )
    , quotasUpdated_( false ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink destructor
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
LogisticLink::~LogisticLink()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ReadQuota
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
void LogisticLink::ReadQuota( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "name", strType );

    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( strType );
    if( !pDotationCategory )
        throw MASA_EXCEPTION( xis.context() + "Unknown dotation type" );

    if( quotas_.find( pDotationCategory ) != quotas_.end() )
        throw MASA_EXCEPTION( xis.context() + "Quota already defined" );

    double quantity;
    xis >> xml::attribute( "quantity", quantity );
    if( quantity < 0 )
        throw MASA_EXCEPTION( xis.context() + "quantity is not greater or equal to 0" );

    sDotationQuota quota;
    quota.quota_          = quantity;
    quota.quotaThreshold_ = quantity * 0.1; //$$ fichier de conf cpp ;)
    quotas_[ pDotationCategory ] = quota;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::WriteQuotas
// Created: NLD 2011-02-03
// -----------------------------------------------------------------------------
void LogisticLink::WriteQuotas( xml::xostream& xos ) const
{
    if( quotas_.empty() )
        return;
    xos << xml::start( "quotas" );
    for( auto it = quotas_.begin(); it != quotas_.end(); ++it )
    {
       const PHY_DotationCategory& dotation = *it->first;
       xos << xml::start( "resource" )
               << xml::attribute( "name", dotation.GetName()  )
               << xml::attribute( "quantity", it->second.quota_ )
           << xml::end; // dotation
    }
    xos << xml::end; // quotas
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ConsumeQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
double LogisticLink::ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quantity, const T_Requesters& requesters )
{
    if( useQuotas_ )
    {
        auto it = quotas_.find( &dotationCategory );
        if( it == quotas_.end() )
            quantity = 0;
        else
        {
            sDotationQuota& quota = it->second;
            const double oldQuota = quota.quota_;
            quantity = std::min( quantity, quota.quota_ );
            if( quantity > 0 )
            {
                quota.quota_ -= quantity;
                quotasUpdated_ = true;
            }
            else
                owner_->NotifyQuotaExceeded( dotationCategory, ComputeNotifications( quota.notifications_, requesters ) );
            if( quota.quota_ <= quota.quotaThreshold_ && oldQuota > quota.quotaThreshold_ )
                owner_->NotifyQuotaThresholdReached( dotationCategory );
        }
    }
    return quantity;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ComputeNotifications
// Created: MCO 2012-12-03
// -----------------------------------------------------------------------------
LogisticLink::T_Requesters LogisticLink::ComputeNotifications( T_Requesters& notifications, T_Requesters requesters ) const
{
    T_Requesters result;
    boost::set_difference( boost::sort( requesters ), notifications, std::back_inserter( result ) );
    boost::copy( result, std::back_inserter( notifications ) );
    boost::sort( notifications );
    return result;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ReturnQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void LogisticLink::ReturnQuota( const PHY_DotationCategory& dotationCategory, double quantity )
{
    if( useQuotas_ )
    {
        auto it = quotas_.find( &dotationCategory );
        if( it != quotas_.end() )
            it->second.quota_ += quantity;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::SendFullState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticLink::SendFullState( unsigned int context ) const
{
    SendQuotas( context );
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::SendChangedState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticLink::SendChangedState() const
{
    if( quotasUpdated_ )
    {
        SendQuotas();
        quotasUpdated_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::SendQuotas
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticLink::SendQuotas( unsigned int context ) const
{
    if( quotas_.empty() )
        return;

    client::LogSupplyQuotas message;
    owner_->Serialize( *message().mutable_supplied() );
    superior_->Serialize( *message().mutable_supplier() );

    sword::SeqOfDotationQuota& msgQuotas = *message().mutable_quotas();
    for( auto it = quotas_.begin(); it != quotas_.end(); ++it )
    {
        sword::DotationQuota& dotQuota = *msgQuotas.add_elem();
        dotQuota.mutable_resource()->set_id( it->first->GetMosID() );
        dotQuota.set_quantity( (unsigned int)it->second.quota_ );
    }

    message.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::OnReceiveChangeQuotas
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
std::set< const PHY_DotationCategory* > LogisticLink::OnReceiveChangeQuotas( const sword::MissionParameter& message )
{
    std::set< const PHY_DotationCategory* > quotasTypes;
    for( int i = 0; i < message.value_size(); ++i )
    {
        const unsigned int type = message.value( i ).list( 0 ).identifier();
        const int number = message.value( i ).list( 1 ).quantity();
        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( type );
        if( pDotationCategory )
        {
            quotasTypes.insert( pDotationCategory );
            sDotationQuota& quota = quotas_[ pDotationCategory ];
            quota.quota_          = number;
            quota.quotaThreshold_ = number * 0.1; //$$ fichier de conf cpp ;)
            quota.notifications_.clear();
        }
    }
    if( !quotasTypes.empty() )
        quotasUpdated_ = true;
    return quotasTypes;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::WriteODB
// Created: NLD 2011-02-03
// -----------------------------------------------------------------------------
void LogisticLink::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "logistic-base" )
            << xml::attribute( "id", superior_->GetLogisticId() )
            << xml::start( "subordinate" )
                    << xml::attribute( "id", owner_->GetLogisticId() );
    WriteQuotas( xos );
    xos     << xml::end
        << xml::end;
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        void serialize( Archive& file, LogisticLink::T_DotationQuotas& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion);
        }

        template < typename Archive >
        void save( Archive& file, const LogisticLink::T_DotationQuotas& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( auto it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second.quota_;
                file << it->second.quotaThreshold_;
            }
        }

        template < typename Archive >
        void load( Archive& file, LogisticLink::T_DotationQuotas& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while( nNbr-- )
            {
                const PHY_DotationCategory* pCategory;
                file >> pCategory;
                LogisticLink::sDotationQuota quota;
                file >> quota.quota_;
                file >> quota.quotaThreshold_;

                map.insert( std::make_pair( pCategory, quota ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
void LogisticLink::serialize( MIL_CheckPointOutArchive& file, const unsigned int )
{
    file << boost::serialization::base_object< LogisticLink_ABC >( *this );
    file << quotas_;
    file << owner_;
    file << superior_;
    file << useQuotas_;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
void LogisticLink::serialize( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< LogisticLink_ABC >( *this );
    file >> quotas_;
    file >> const_cast< LogisticHierarchyOwner_ABC*& >( owner_ );
    file >> superior_;
    file >> const_cast< bool& >( useQuotas_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::GetSuperior
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
MIL_AutomateLOG& LogisticLink::GetSuperior() const
{
    return *superior_;
}
