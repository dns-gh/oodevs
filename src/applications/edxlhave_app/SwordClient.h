// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SwordClient_h_
#define __SwordClient_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <boost/asio.hpp>
#include <boost/function.hpp>

#pragma warning( push, 0 )
#include "proto/client_authentication.pb.h"
#include "proto/authentication_client.pb.h"
#include "proto/aar_client.pb.h"
#include "proto/dispatcher_client.pb.h"
#include "proto/messenger_client.pb.h"
#include "proto/replay_client.pb.h"
#include "proto/simulation_client.pb.h"
#include "proto/client_simulation.pb.h"
#pragma warning( pop )

namespace sword
{
    // message identifiers
    namespace tag
    {
        const unsigned long client_authentication = 0xCA3E8777;
        const unsigned long client_messenger      = 0x9E03C338;
        const unsigned long client_simulation     = 0x1D973941;
        const unsigned long authentication_client = 0x1BE91304;
        const unsigned long aar_client            = 0xC6CDD5C6;
        const unsigned long dispatcher_client     = 0xB38DC355;
        const unsigned long messenger_client      = 0xFF2B3A46;
        const unsigned long replay_client         = 0xA85B7F22;
        const unsigned long simulation_client     = 0xD305BD09;
    }
}

// =============================================================================
/** @class  SwordClient
    @brief  send/receive network messages to a sword simulation instance
*/
// Created: BCI 2011-04-29
// =============================================================================
class SwordClient : boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SwordClient();
    virtual ~SwordClient();
    //@}

    //! @name Operations
    //@{
     void Connect( const std::string& hostname, const std::string& port, bool waitconnection );
     void Send( const sword::ClientToAuthentication& message );
     void Send( const sword::ClientToSim& message );
     bool Receive();

     void SetListener( boost::function< void( const sword::SimToClient& ) > listener );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Message >
    void ReadMessage( const std::vector< google::protobuf::uint8 >& buffer );

    template< typename Message >
    void Handle( const Message& );

    template<>
    void Handle( const sword::SimToClient& );

    template< typename Message >
    void Send( const Message& message, unsigned long tag );
    //@}

private:
    //! @name Member data
    //@{
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    boost::function< void( const sword::SimToClient& ) > simToClientHandler_;
    unsigned int bytes_;
    //@}
};

#endif // __SwordClient_h_
