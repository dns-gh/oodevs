// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "LogisticHierarchy.h"
#include "MIL_AutomateLOG.h"
#include "LogisticLink.h"
#include "LogisticHierarchyOwner_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Iterator.h>
#include <boost/serialization/deque.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace logistic;

BOOST_CLASS_EXPORT_IMPLEMENT( LogisticHierarchy )

template< typename Archive >
void logistic::save_construct_data( Archive& archive, const logistic::LogisticHierarchy* logisticHierarchy, const unsigned int )
{
    archive << logisticHierarchy->owner_;
    archive << logisticHierarchy->useQuotas_;
}

template< typename Archive >
void logistic::load_construct_data( Archive& archive, logistic::LogisticHierarchy* logisticHierarchy, const unsigned int )
{
    LogisticHierarchyOwner_ABC* owner;
    bool useQuotas;
    archive >> owner >> useQuotas;
    ::new(logisticHierarchy)LogisticHierarchy( *owner, useQuotas );
}

// =============================================================================
// $$$ A DEPLACER
// =============================================================================

namespace logistic {

// =============================================================================
/** @class  LogisticSuperiorIterator
    @brief  Simple container reverse iterator
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Container >
class LogisticSuperiorIterator : public tools::Iterator_ABC< MIL_AutomateLOG& >
{
public:
    LogisticSuperiorIterator( const Container& container )
        : begin_  ( container.rbegin() )
        , current_( container.rbegin() )
        , end_    ( container.rend() )
    {}
    virtual bool HasMoreElements() const
    {
        return current_ != end_;
    }

    virtual MIL_AutomateLOG& NextElement()
    {
        MIL_AutomateLOG& result = (**current_).GetSuperior();
        ++current_;
        return result;
    }

    virtual void Rewind()
    {
        current_ = begin_;
    }

private:
    LogisticSuperiorIterator( const LogisticSuperiorIterator& );
    LogisticSuperiorIterator& operator=( LogisticSuperiorIterator& );

    typename Container::const_reverse_iterator begin_;
    typename Container::const_reverse_iterator current_;
    typename Container::const_reverse_iterator end_;
};

// =============================================================================
/** @class  SimpleReverseIterator
    @brief  Simple container iterator
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename Item, typename Container >
class SimpleReverseIterator : public tools::Iterator_ABC< Item >
{
public:
    SimpleReverseIterator( const Container& container )
        : current_( container.rbegin() )
        , end_( container.rend() )
    {}
    virtual bool HasMoreElements() const
    {
        return current_ != end_;
    }
    virtual Item NextElement()
    {
        return *current_++;
    }
private:
    SimpleReverseIterator( const SimpleReverseIterator& );
    SimpleReverseIterator& operator=( SimpleReverseIterator& );

    typename Container::const_reverse_iterator current_;
    typename Container::const_reverse_iterator end_;
};

}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
LogisticHierarchy::LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, bool useQuotas )
    : owner_       ( &owner )
    , useQuotas_   ( useQuotas )
    , linksUpdated_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
LogisticHierarchy::LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& nominalSuperior, bool useQuotas )
    : owner_       ( &owner )
    , useQuotas_   ( useQuotas )
    , linksUpdated_( true )
{
    superiorLinks_.push_back( boost::make_shared< LogisticLink >( owner, nominalSuperior, useQuotas ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
LogisticHierarchy::LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& nominalSuperior, bool useQuotas, xml::xistream& xis )
    : owner_       ( &owner )
    , useQuotas_   ( useQuotas )
    , linksUpdated_( true )
{
    superiorLinks_.push_back( boost::make_shared< LogisticLink >( owner, nominalSuperior, useQuotas, xis ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy destructor
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
LogisticHierarchy::~LogisticHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::CreateSuperiorLinksIterator
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
tools::Iterator< boost::shared_ptr< LogisticLink_ABC > > LogisticHierarchy::CreateSuperiorLinksIterator() const
{
    return new SimpleReverseIterator< boost::shared_ptr< LogisticLink_ABC >, T_SuperiorLinks >( superiorLinks_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::CreateSuperiorsIterator
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
tools::Iterator< MIL_AutomateLOG& > LogisticHierarchy::CreateSuperiorsIterator() const
{
    return new LogisticSuperiorIterator< T_SuperiorLinks >( superiorLinks_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::FindSuperiorLink
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
const boost::shared_ptr< LogisticLink_ABC > LogisticHierarchy::FindSuperiorLink( const MIL_AutomateLOG& superior ) const
{
    for( auto it = superiorLinks_.rbegin(); it != superiorLinks_.rend(); ++it )
    {
        const auto& link = *it;
        if( &link->GetSuperior() == &superior )
            return link;
    }
    return boost::shared_ptr< LogisticLink_ABC >();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::HasSuperior
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
bool LogisticHierarchy::HasSuperior() const
{
    return !superiorLinks_.empty();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::GetPrimarySuperior
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
MIL_AutomateLOG* LogisticHierarchy::GetPrimarySuperior() const
{
    if( superiorLinks_.empty() )
        return 0;
    return &superiorLinks_.back()->GetSuperior();
}

namespace
{
    bool HasSameSuperior( const boost::shared_ptr< LogisticLink_ABC >& lhs, const boost::shared_ptr< LogisticLink_ABC >& rhs )
    {
        return &lhs->GetSuperior() == &rhs->GetSuperior();
    }

    class LinkNotifier : boost::noncopyable
    {
    private:
        typedef std::deque< boost::shared_ptr< LogisticLink_ABC > > T_SuperiorLinks;

    public:
        LinkNotifier( T_SuperiorLinks& links, const LogisticHierarchyOwner_ABC& owner )
            : initial_( links )
            , final_  ( links )
            , owner_  ( owner )
        {}
        void Notify() const
        {
            BOOST_FOREACH( const auto& link, initial_ )
                if( boost::find_if( final_, boost::bind( &HasSameSuperior, link, _1 ) ) == final_.end() )
                    owner_.NotifyLinkRemoved( *link );
            BOOST_FOREACH( const auto& link, final_ )
                if( boost::find_if( initial_, boost::bind( &HasSameSuperior, link, _1 ) ) == initial_.end() )
                    owner_.NotifyLinkAdded( *link );
        }
    private:
        const T_SuperiorLinks initial_;
        const T_SuperiorLinks& final_;
        const LogisticHierarchyOwner_ABC& owner_;
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SwitchToHierarchy
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void LogisticHierarchy::SwitchToHierarchy( const LogisticHierarchy_ABC& newHierarchy )
{
    LinkNotifier notifier( superiorLinks_, *owner_ );
    backupSuperiorLinks_ = superiorLinks_;
    superiorLinks_.clear();
    tools::Iterator< MIL_AutomateLOG& > it = newHierarchy.CreateSuperiorsIterator();
    while( it.HasMoreElements() )
        superiorLinks_.push_front( boost::make_shared< LogisticLink >( *owner_, it.NextElement(), useQuotas_ ) );
    linksUpdated_ = true;
    notifier.Notify();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::DisconnectFromHierarchy
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticHierarchy::DisconnectFromHierarchy()
{
    LinkNotifier notifier( superiorLinks_, *owner_ );
    // Keep the oldest backup links
    if( backupSuperiorLinks_.empty() )
        backupSuperiorLinks_ = superiorLinks_;
    superiorLinks_.clear();
    linksUpdated_ = true;
    notifier.Notify();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SwitchBackToNominalHierarchy
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void LogisticHierarchy::SwitchBackToNominalHierarchy()
{
    LinkNotifier notifier( superiorLinks_, *owner_ );
    superiorLinks_ = backupSuperiorLinks_;
    backupSuperiorLinks_.clear();
    linksUpdated_ = true;
    notifier.Notify();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::ChangeLinks
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void LogisticHierarchy::ChangeLinks( const std::vector< MIL_AutomateLOG* >& superiors )
{
    LinkNotifier notifier( superiorLinks_, *owner_ );
    superiorLinks_.clear();
    BOOST_FOREACH( MIL_AutomateLOG* superior, superiors )
        if( superiorLinks_.empty() || &superiorLinks_.back()->GetSuperior() != superior )
            superiorLinks_.push_back( boost::make_shared< LogisticLink >( *owner_, *superior, useQuotas_ ) );
    linksUpdated_ = true;
    notifier.Notify();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::WriteODB
// Created: NLD 2011-02-03
// -----------------------------------------------------------------------------
void LogisticHierarchy::WriteODB( xml::xostream& xos ) const
{
    if( !superiorLinks_.empty() )
        superiorLinks_.back()->WriteODB( xos );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SendFullState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticHierarchy::SendFullState( unsigned int context ) const
{
    SendLinks( context );
    boost::for_each( superiorLinks_, boost::bind( &LogisticLink_ABC::SendFullState, _1, context ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SendChangedState
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticHierarchy::SendChangedState() const
{
    if( linksUpdated_ )
    {
        SendFullState(); // Send full state of links, including quotas
        linksUpdated_ = false;
    }
    else
        // Links attributes
        boost::for_each( superiorLinks_, std::mem_fn( &LogisticLink_ABC::SendChangedState ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::SendLinks
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
void LogisticHierarchy::SendLinks( unsigned int context ) const
{
    client::ChangeLogisticLinks message;
    owner_->Serialize( *message().mutable_requester() );
    BOOST_FOREACH( boost::shared_ptr< LogisticLink_ABC > link, superiorLinks_ )
        link->GetSuperior().Serialize( *message().add_superior() );
    message.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
template < typename Archive >
void LogisticHierarchy::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< LogisticHierarchy_ABC >( *this );
    file & superiorLinks_;
//    file & backupSuperiorLinks_; // FIXME
}
