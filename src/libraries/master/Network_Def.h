// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NetworkDef_h__
#define __NetworkDef_h__

#include "Publisher_ABC.h"
#include "master_asn/Asn.h"

namespace master
{
//! @name DIN Messages
//@{
static enum  
{
    eMsgOutMaster   = 0,
    eMsgInMaster    = 1
};
//@}

 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )  \
class AsnMsg##TARGET##ASNMSG                                     \
{                                                                \
public:                                                          \
    AsnMsg##TARGET##ASNMSG() {}                                  \
                                                                 \
    void Send( Publisher_ABC& publisher )                        \
    {                                                            \
        ASN1T_MsgsIn##TARGET asnMsg;                             \
        asnMsg.msg.t = T_Msgs##TARGET##_msg_msg_##ASNVAR;        \
        publisher.Send( asnMsg );                                \
    }                                                            \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )           \
class AsnMsg##TARGET##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    AsnMsg##TARGET##ASNMSG() {}                                           \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_MsgsIn##TARGET asnMsg;                                      \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                               \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                 \
class AsnMsg##TARGET##ASNMSG                                              \
{                                                                         \
public:                                                                   \
    AsnMsg##TARGET##ASNMSG() {}                                           \
                                                                          \
    void Send( Publisher_ABC& publisher )                                 \
    {                                                                     \
        ASN1T_Msgs##TARGET asnMsg;                                        \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;    \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                              \
        publisher.Send( asnMsg );                                         \
    }                                                                     \
                                                                          \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                    \
private:                                                                  \
    ASN1T_Msg##ASNMSG asnTmp;                                             \
};

GENERATE_ASN_MSG_SENDER( OutMaster, AuthenticationResponse, authentication_response );
GENERATE_ASN_MSG_SENDER( OutMaster, TerrainCreation       , terrain_creation        );
GENERATE_ASN_MSG_SENDER( OutMaster, PhysicalModelCreation , physical_model_creation );
GENERATE_ASN_MSG_SENDER( OutMaster, DatasetCreation       , dataset_creation        );
GENERATE_ASN_MSG_SENDER( OutMaster, ExerciseCreation      , exercise_creation       );

/*
template< typename L, typename E, typename P > 
void SendContainerValues( const P& container, L& asnList )
{
    asnList.n = container.size();
    if( !container.empty() )
    {
        asnList.elem = new E[ container.size() ];
        uint i = 0;
        for( P::const_iterator it = container.begin(); it != container.end(); ++it, ++i )
            asnList.elem[ i ] = *it;
    }
}

template< typename L, typename E, typename P > 
void SendContainerRefs( const P& container, L& asnList )
{
    asnList.n = container.size();
    if( !container.empty() )
    {
        asnList.elem = new E[ container.size() ];
        uint i = 0;
        for( P::const_iterator it = container.begin(); it != container.end(); ++it, ++i )
            asnList.elem[ i ] = (**it).GetID();
    }
}
*/

} // end namespace master

#endif // __NetworkDef_h__
