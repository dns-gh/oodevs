// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef _MSC_VER
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif
#endif

#include "IpcQueue.h"

#include "IpcHandler_ABC.h"
#include "ThreadPool.h"
#include "WaitEvent.h"

#include "MT_Tools/MT_Logger.h"

#include <boost/function.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>

using namespace tools::ipc;
namespace bip = boost::interprocess;

namespace
{
    const size_t max_messages = 64;
    const size_t message_size = sizeof( Command );
    const size_t timeout_ms = 100;
    uint32_t num_queues = 0;

    template< typename T >
    std::string GetQueueName( T pid )
    {
        return "ipc_" + boost::lexical_cast< std::string >( pid );
    }

    std::string GetQueueName()
    {
        return GetQueueName( bip::ipcdetail::get_current_process_id() );
    }

    bip::message_queue* MakeQueue()
    {
        const std::string name = GetQueueName();
        bip::message_queue::remove( name.c_str() );
        return new bip::message_queue( bip::create_only, name.c_str(),
                                       max_messages, message_size );
    }
}

Queue::Queue( Handler_ABC& handler )
    : handler_( handler )
    , pool_   ( new tools::ThreadPool( 1 ) )
    , quit_   ( new tools::WaitEvent() )
    , queue_  ( MakeQueue() )
{
    const bool first = !bip::ipcdetail::atomic_inc32( &num_queues );
    if( !first )
        throw MASA_EXCEPTION( "Unable to start multiple IPC queues" );
    pool_->Post( boost::bind( &Queue::Run, this ) );
}

Queue::~Queue()
{
    quit_->Signal();
    pool_->Join( tools::ThreadPool::ProcessPending );
    try
    {
        bip::message_queue::remove( GetQueueName().c_str() );
        bip::ipcdetail::atomic_dec32( &num_queues );
    }
    catch( const std::exception& err )
    {
        MT_LOG_ERROR_MSG( std::string( "Unable to remove message queue " )
                          + err.what() );
    }
}

void Queue::Run()
{
    while( !quit_->IsSignaled() )
        try
        {
            Update();
        }
        catch( std::exception& err )
        {
            MT_LOG_WARNING_MSG( std::string( "Skipping invalid command " )
                                + err.what() );
        }
}

namespace
{
uint32_t read_u32be( const void* data, size_t size )
{
    (void)size;
    assert( size >= sizeof(uint32_t) );
    const char* ptr = reinterpret_cast< const char* >( data );
    return uint32_t( ptr[0] ) << 24
         | uint32_t( ptr[1] ) << 16
         | uint32_t( ptr[2] ) << 8
         | uint32_t( ptr[3] );
}

void write_u32be( void* data, size_t size, uint32_t value )
{
    (void)size;
    assert( size >= sizeof(uint32_t) );
    char* ptr = reinterpret_cast< char* >( data );
    ptr[0] = uint8_t( value >> 24 & 0xFF );
    ptr[1] = uint8_t( value >> 16 & 0xFF );
    ptr[2] = uint8_t( value >> 8  & 0xFF );
    ptr[3] = uint8_t( value       & 0xFF );
}

BOOST_STATIC_ASSERT( sizeof(Command) == sizeof(uint32_t) );

// use protobuf the day we need something more than one integer
Command Unpack( const void* data, size_t size )
{
    return static_cast< Command >( read_u32be( data, size ) );
}

void Pack( void* data, size_t size, Command cmd )
{
    write_u32be( data, size, cmd );
}
}

void Queue::Update()
{
    char buffer[sizeof(Command)];
    size_t read;
    unsigned priority;
    const auto timeout = boost::posix_time::microsec_clock::universal_time()
                       + boost::posix_time::milliseconds( timeout_ms );
    const bool found = queue_->timed_receive( buffer, sizeof buffer, read,
                                              priority, timeout );
    if( !found )
        return;
    if( read != sizeof buffer )
    {
        MT_LOG_WARNING_MSG( "Skipping invalid command ("
                            + boost::lexical_cast< std::string >( read )
                            + " bytes)" );
        return;
    }
    Process( Unpack( buffer, sizeof buffer ) );
}

void Queue::Process( Command cmd )
{
    switch( cmd )
    {
        case IPC_COMMAND_STOP:  return handler_.Stop();
    }
    MT_LOG_WARNING_MSG( "Skipping invalid command "
                        + boost::lexical_cast< std::string >( cmd ) );
}

bool Queue::Send( Command cmd, int pid )
{
    try
    {
        char buffer[sizeof cmd];
        Pack( buffer, sizeof buffer, cmd );
        bip::message_queue dst( bip::open_only, GetQueueName( pid ).c_str() );
        dst.send( buffer, sizeof buffer, 0 );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}
