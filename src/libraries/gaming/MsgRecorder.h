// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MsgRecorder_h_
#define __MsgRecorder_h_

#include "game_asn/Asn.h"
#include "boost/thread/mutex.hpp"
class AgentServerMsgMgr;
class Network;

namespace internal
{
    class Message;
};

// =============================================================================
/** @class  MsgRecorder
    @brief  MsgRecorder
*/
// Created: APE 2004-10-20
// =============================================================================
class MsgRecorder
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MsgRecorder( Network& network );
    virtual ~MsgRecorder();
    //@}

    //! @name Operations
    //@{
    void OnNewMsg( int nType, ASN1PEREncodeBuffer& asnPEREncodeBuffer );
    void Play( const std::string& filename );

    void Record();
    bool Stop();

    void Write( const std::string& filename );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MsgRecorder( const MsgRecorder& );
    MsgRecorder& operator=( const MsgRecorder& );
    //@}
    
private:
    //! @name Types
    //@{
    typedef std::vector< internal::Message* >  T_Messages;
    typedef T_Messages::iterator              IT_Messages;
    typedef T_Messages::const_iterator       CIT_Messages;
    //@}

private:
    //! @name Member data
    //@{
    AgentServerMsgMgr& msgManager_;
    boost::mutex mutex_;
    bool recording_;
    T_Messages messages_;
    //@}
};

#endif // __MsgRecorder_h_
