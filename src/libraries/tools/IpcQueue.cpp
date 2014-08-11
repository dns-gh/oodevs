// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "IpcQueue.h"
#include "IpcHandler_ABC.h"
#include "ThreadPool.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/bind.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/lexical_cast.hpp>

using namespace tools::ipc;
namespace bip = boost::interprocess;

namespace
{
    const size_t max_messages = 64;
    const size_t message_size = sizeof( Command );
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

struct tools::ipc::Queue::Counter : public boost::noncopyable
{
    Counter()
        : value( bip::ipcdetail::atomic_inc32( &num_queues ) )
    {
        // NOTHING
    }
    ~Counter()
    {
        bip::ipcdetail::atomic_dec32( &num_queues );
    }
    const uint32_t value;
};

Queue::Queue( Handler_ABC& handler )
    : handler_( handler )
    , pool_   ( new tools::ThreadPool( 1 ) )
    , counter_( new Counter() )
{
    if( counter_->value > 0 )
        throw MASA_EXCEPTION( "Unable to start multiple IPC queues" );
    queue_.reset( MakeQueue() );
    pool_->Post( boost::bind( &Queue::Run, this ) );
}

Queue::~Queue()
{
    Send( *queue_, IPC_COMMAND_EXIT );
    pool_->Join( tools::ThreadPool::ProcessPending );
    try
    {
        bip::message_queue::remove( GetQueueName().c_str() );
    }
    catch( const std::exception& err )
    {
        MT_LOG_ERROR_MSG( "Unable to remove message queue " << err.what() );
    }
}

void Queue::Run()
{
    for( ;; )
        try
        {
            const bool last = Update();
            if( last )
                return;
        }
        catch( std::exception& err )
        {
            MT_LOG_WARNING_MSG( "Skipping invalid command " << err.what() );
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

static_assert( sizeof(Command) == sizeof(uint32_t), "invalid Command size" );

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

bool Queue::Update()
{
    char buffer[sizeof(Command)];
    size_t read;
    unsigned priority;
    queue_->receive( buffer, sizeof buffer, read, priority );
    if( read != sizeof buffer )
    {
        MT_LOG_WARNING_MSG( "Skipping invalid command ("
            << read << " bytes)" );
        return false;
    }
    return Process( Unpack( buffer, sizeof buffer ) );
}

bool Queue::Process( Command cmd )
{
    switch( cmd )
    {
        case IPC_COMMAND_STOP:
            handler_.Stop();
            return false;

        case IPC_COMMAND_EXIT:
            return true;
    }
    MT_LOG_WARNING_MSG( "Skipping invalid command " << cmd );
    return false;
}

bool Queue::Send( bip::message_queue& queue, Command cmd )
{
    try
    {
        char buffer[sizeof cmd];
        Pack( buffer, sizeof buffer, cmd );
        queue.send( buffer, sizeof buffer, 0 );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

bool Queue::Send( Command cmd, int pid )
{
    try
    {
        if( cmd == IPC_COMMAND_EXIT )
            return false;
        bip::message_queue queue( bip::open_only, GetQueueName( pid ).c_str() );
        return Send( queue, cmd );
    }
    catch( ... )
    {
        return false;
    }
}
