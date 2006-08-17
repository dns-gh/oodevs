// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MsgRecorder.h $
// $Author: Ape $
// $Modtime: 21/10/04 10:36 $
// $Revision: 1 $
// $Workfile: MsgRecorder.h $
//
// *****************************************************************************

#ifndef __MsgRecorder_h_
#define __MsgRecorder_h_

#include "ASN_Types.h"
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
    void OnNewMsgWithContext( int nType, unsigned long nContext, ASN1PEREncodeBuffer& asnPEREncodeBuffer );

    void Play();
    void Record();
    bool Stop();
    void Clear();

    void Read( const std::string& filename );
    void Write( const std::string& filename ) const;
    //@}

private:
    //! @name Helpers
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

    bool recording_;
    T_Messages messages_;
    //@}
};

#endif // __MsgRecorder_h_
