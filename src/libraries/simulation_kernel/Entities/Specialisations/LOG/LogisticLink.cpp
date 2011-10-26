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
#include "Network/NET_AsnException.h"
#include "protocol/ClientSenders.h"

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticLink )

template< typename Archive >
void logistic::save_construct_data( Archive& archive, const logistic::LogisticLink* link, const unsigned int )
{
    archive << link->owner_
            << link->superior_
            << link->useQuotas_;
}

template< typename Archive >
void logistic::load_construct_data( Archive& archive, logistic::LogisticLink* link, const unsigned int )
{
    LogisticHierarchyOwner_ABC* owner;
    MIL_AutomateLOG* superior;
    bool useQuotas;
    archive >> owner
            >> superior
            >> useQuotas;
    ::new(link)LogisticLink( *owner, *superior, useQuotas );
}

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
        xis.error( "Unknown dotation type" );

    if( quotas_.find( pDotationCategory ) != quotas_.end() )
        xis.error( "Quota already defined" );

    unsigned int quantity;
    xis >> xml::attribute( "quantity", quantity );
    if( quantity < 0 )
        xis.error( "quantity is not greater or equal to 0" );

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
    for( T_DotationQuotas::const_iterator it = quotas_.begin(); it != quotas_.end(); ++it )
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
// Name: LogisticLink::operator==
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
bool LogisticLink::operator==( const LogisticLink& rhs ) const
{
    return &superior_ == &rhs.superior_ && quotas_ == rhs.quotas_ && useQuotas_ == rhs.useQuotas_;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ConsumeQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
double LogisticLink::ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quantity )
{
    if( useQuotas_ )
    {
        T_DotationQuotas::iterator it = quotas_.find( &dotationCategory );
        if( it == quotas_.end() )
            quantity = 0;
        else
        {
            sDotationQuota& quota = it->second;
            quantity = std::min( quantity, quota.quota_ );
            if( quantity > 0 )
            {
                quota.quota_ -= quantity;
                quotasUpdated_ = true;
            }
            if( quota.quota_ <= quota.quotaThreshold_ )
                owner_->NotifyQuotaThresholdReached( dotationCategory );
        }
    }
    return quantity;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::ConsumeQuota
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
void LogisticLink::ReturnQuota( const PHY_DotationCategory& dotationCategory, double quantity )
{
    if( useQuotas_ )
    {
        T_DotationQuotas::iterator it = quotas_.find( &dotationCategory );
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
    for( T_DotationQuotas::const_iterator it = quotas_.begin(); it != quotas_.end(); ++it )
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
void LogisticLink::OnReceiveChangeQuotas( const sword::MissionParameter& message )
{
    for( int i = 0; i < message.value_size(); ++i )
    {
        unsigned int type = message.value( i ).list( 0 ).identifier();
        int number = message.value( i ).list( 1 ).quantity();

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( type );
        if( !pDotationCategory )
            throw NET_AsnException< sword::LogSupplyChangeQuotasAck_ErrorCode >( sword::LogSupplyChangeQuotasAck_ErrorCode_error_invalid_dotation );

        sDotationQuota& quota = quotas_[ pDotationCategory ];
        quota.quota_          = number;
        quota.quotaThreshold_ = number * 0.1; //$$ fichier de conf cpp ;)
    }
    quotasUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::WriteODB
// Created: NLD 2011-02-03
// -----------------------------------------------------------------------------
void LogisticLink::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "logistic-base" )
            << xml::attribute( "id", superior_->GetID() )
            << xml::start( "subordinate" )
                    << xml::attribute( "id", owner_->GetID() );
    WriteQuotas( xos );
    xos     << xml::end
        << xml::end;
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline void serialize( Archive& file, LogisticLink::T_DotationQuotas& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion);
        }

        template < typename Archive >
        void save( Archive& file, const LogisticLink::T_DotationQuotas& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( LogisticLink::T_DotationQuotas::const_iterator it = map.begin(); it != map.end(); ++it )
            {
                unsigned int id = it->first->GetMosID();
                file << id
                     << it->second.quota_
                     << it->second.quotaThreshold_;
            }
        }

        template < typename Archive >
        void load( Archive& file, LogisticLink::T_DotationQuotas& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nCategory;
                file >> nCategory;
                const PHY_DotationCategory* pCategory = PHY_DotationType::FindDotationCategory( nCategory );

                LogisticLink::sDotationQuota quota;
                file >> quota.quota_
                     >> quota.quotaThreshold_;

                map.insert( std::make_pair( pCategory, quota ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
template < typename Archive >
void LogisticLink::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< LogisticLink_ABC >( *this )
         & quotas_;
}
