// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher/ClientPublisher_ABC.h"

#include "game_asn/Messenger.h"

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    ASN_Msg##ASNMSG() {}                                                  \
    ASN_Msg##ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                \
                                                                          \
    void Send(dispatcher::ClientPublisher_ABC& client)                    \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;            \
        asnMsg.u.msg_##ASNVAR = &asnTmp;                                  \
        client.Send(asnMsg);                                              \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class ASN_Msg##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    ASN_Msg##ASNMSG() {}                                                  \
    ASN_Msg##ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                \
                                                                          \
    void Send(dispatcher::ClientPublisher_ABC& client)                    \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;            \
        asnMsg.u.msg_##ASNVAR = asnTmp;                                   \
        client.Send(asnMsg);                                              \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
                                                                          \
};

// LIMIT
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimitCreationRequestAck,   limit_creation_request_ack)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimitDestructionRequestAck,limit_destruction_request_ack )
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimitUpdateRequestAck,     limit_update_request_ack)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, LimitCreation,             limit_creation)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, LimitUpdate,               limit_update)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimitDestruction,          limit_destruction)

// LIMA
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimaCreationRequestAck,    lima_creation_request_ack)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimaDestructionRequestAck, lima_destruction_request_ack)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimaUpdateRequestAck,      lima_update_request_ack)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, LimaCreation,              lima_creation)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, LimaUpdate,                lima_update)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, LimaDestruction,           lima_destruction)

// INTELLIGENCE
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, IntelligenceCreationRequestAck,    intelligence_creation_request_ack)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, IntelligenceDestructionRequestAck, intelligence_destruction_request_ack)
GENERATE_NOPTR_ASN_MSG_SENDER(MessengerToClient, IntelligenceUpdateRequestAck,      intelligence_update_request_ack)

GENERATE_ASN_MSG_SENDER      (MessengerToClient, IntelligenceCreation,              intelligence_creation)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, IntelligenceUpdate,                intelligence_update)
GENERATE_ASN_MSG_SENDER      (MessengerToClient, IntelligenceDestruction,           intelligence_destruction)
