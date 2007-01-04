// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Network_h_
#define __Network_h_

#undef Yield
#include "tools/thread/Thread.h"
#include <boost/thread/mutex.hpp>

class AgentServerMsgMgr;
class Model;
class Simulation;
class Profile;

namespace kernel
{
    class Controllers;
}

namespace DIN
{
    class DIN_Link;
    class DIN_Engine;
    class DIN_ErrorDescription;
    template< typename T > class DIN_ConnectionServiceClientUserCbk;
}

// =============================================================================
/** @class  Network
    @brief  Network
*/
// Created: AGE 2006-02-08
// =============================================================================
class Network : public tools::thread::Thread
{

public:
    //! @name Constructors/Destructor
    //@{
             Network( kernel::Controllers& controllers, Simulation& simu, Profile& profile );
    virtual ~Network();
    //@}

    //! @name Operations
    //@{
    bool Connect( const std::string& strHostName, unsigned short nPort );
    bool Disconnect();
    bool IsConnected() const;

    void Update();

    AgentServerMsgMgr& GetMessageMgr();
    //@}

    //! @name Statistics
    //@{
    unsigned long GetReceivedAmount() const;
    unsigned long GetSentAmount() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Network( const Network& );            //!< Copy constructor
    Network& operator=( const Network& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Run();
    void OnConnected     ( DIN::DIN_Link& link );
    void OnNotConnected  ( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Types
    //@{
    struct T_Message
    {
        DIN::DIN_Link* link_;
        std::string address_;
        std::string error_;
        bool lost_;
    };
    typedef std::vector< T_Message >     T_Messages;
    typedef T_Messages::const_iterator CIT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    boost::mutex mutex_;
    T_Messages messages_;
    Simulation& simu_;
    Profile& profile_;
    DIN::DIN_Engine* engine_;
    DIN::DIN_ConnectionServiceClientUserCbk< Network >* pConnService_;

    DIN::DIN_Link* session_;
    AgentServerMsgMgr* manager_;
    bool terminated_;
    //@}
};

#endif // __Network_h_
