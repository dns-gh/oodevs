// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "PublisherActor.h"
#include "ResponseHandler_ABC.h"
#undef Yield
#undef GetMessage
#include "tools/thread/ThreadPool.h"
#include <boost/bind.hpp>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: PublisherActor constructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::PublisherActor( std::auto_ptr< Publisher_ABC > base )
    : base_( base )
    , thread_( new tools::thread::ThreadPool( 1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor destructor
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
PublisherActor::~PublisherActor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PushReports()
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoPushReports, this ) );
    base_->PushReports();//TEMP
}
// -----------------------------------------------------------------------------
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
std::string PublisherActor::GetUrlId()
{
  //thread_->Enqueue( boost::bind( &PublisherActor::DoPushReports, this ) );
  return base_->GetUrlId();//TEMP
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PushReport( const std::string& xmliaMessage, const std::string& strPoe )
{
  //thread_->Enqueue( boost::bind( &PublisherActor::DoPushReports, this ) );
  base_->PushReport( xmliaMessage, strPoe );//TEMP
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::CreateReport
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
xml::xostream& PublisherActor::CreateReport()
{
    return base_->CreateReport();
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPushReports
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPushReports()
{
    base_->PushReports();
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::PullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::PullOrder( const std::string& message, ResponseHandler_ABC& handler )
{
    thread_->Enqueue( boost::bind( &PublisherActor::DoPullOrder, this, message, boost::ref( handler ) ) );
}

namespace
{
    struct HandlerWrapper : public ResponseHandler_ABC
    {
        explicit HandlerWrapper( std::string& response ) : response_( &response ) {}
        virtual void Handle( const std::string& response )
        {
            *response_ = response;
        }
        std::string* response_;
    };
}
// -----------------------------------------------------------------------------
// Name: PublisherActor::DoPullOrder
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::DoPullOrder( const std::string& message, ResponseHandler_ABC& handler )
{
    std::string answer;
    HandlerWrapper wrapper( answer );
    base_->PullOrder( message, wrapper );

    boost::recursive_mutex::scoped_lock locker( mutex_ );
    answers_.push_back( T_Answer( &handler, answer ) );
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::CommitOrders
// Created: AGE 2008-05-30
// -----------------------------------------------------------------------------
void PublisherActor::CommitOrders()
{
    T_Answers toDeal;
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        std::swap( answers_, toDeal );
    }
    for( T_Answers::const_iterator it = toDeal.begin(); it != toDeal.end(); ++it )
        it->first->Handle( it->second );
}


// -----------------------------------------------------------------------------
// Name: PublisherActor::GetUrlReports
// Created: SLG 2009-05-30
// -----------------------------------------------------------------------------
std::string PublisherActor::GetUrlReports()
{
  return base_->GetUrlReports();
}

// -----------------------------------------------------------------------------
// Name: PublisherActor::GetUrlReports
// Created: SLG 2009-05-30
// -----------------------------------------------------------------------------
std::string PublisherActor::GetXmliaMessage( const std::string& url )
{
  return base_->GetXmliaMessage( url );
}