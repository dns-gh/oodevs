// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Messages_h_
#define __Messages_h_

#include "Publisher_ABC.h"

namespace frontend
{

static enum
{
    eMsgOutMaster = 0,
    eMsgInMaster  = 1,
    eMsgInit      = 1007
};


#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class AsnMsgIn##TARGET##ASNMSG                                   \
{                                                                \
public:                                                          \
    AsnMsgIn##TARGET##ASNMSG() {}                                \
                                                                 \
    void Send( Publisher_ABC& publisher )                        \
    {                                                            \
        ASN1T_MsgsIn##TARGET asnMsg;                             \
        asnMsg.msg.t = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;      \
        publisher.Send( asnMsg );                                \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class AsnMsgIn##TARGET##ASNMSG                                            \
{                                                                         \
public:                                                                   \
    AsnMsgIn##TARGET##ASNMSG() {}                                         \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_MsgsIn##TARGET asnMsg;                                      \
        asnMsg.msg.t              = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;  \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                               \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                 \
class AsnMsgIn##TARGET##ASNMSG                                            \
{                                                                         \
public:                                                                   \
    AsnMsgIn##TARGET##ASNMSG() {}                                         \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_MsgsIn##TARGET asnMsg;                                      \
        asnMsg.msg.t              = T_MsgsIn##TARGET##_msg_msg_##ASNVAR;  \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

GENERATE_EMPTY_ASN_MSG_SENDER( Master, ClientAnnouncement        , client_announcement    );
GENERATE_ASN_MSG_SENDER      ( Master, AuthenticationRequest     , authentication_request );
GENERATE_ASN_MSG_SENDER      ( Master, ExerciseCreationRequest   , exercise_creation_request );
GENERATE_ASN_MSG_SENDER      ( Master, ExerciseUpdateRequest     , exercise_update_request );
GENERATE_NOPTR_ASN_MSG_SENDER( Master, ExerciseDestructionRequest, exercise_destruction_request );

}

#endif // __Messages_h_
