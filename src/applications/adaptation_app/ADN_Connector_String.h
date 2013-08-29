// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Connector_String_h_
#define __ADN_Connector_String_h_

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
template< class T >
class ADN_Connector_String : public ADN_Connector_ABC
{
public:
    explicit ADN_Connector_String( T* gfx );
    virtual ~ADN_Connector_String();

    void SetDataChanged( const QString& string );
    bool IsConnected() const;

protected:
    void SetDataPrivate( void *data );
    virtual void ConnectPrivateSub( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_ABC* pTarget );
    virtual void connectNotify( const char *signal );
    virtual void disconnectNotify( const char *signal );

protected:
    T* pGfx_;
    bool bIsConnected_;
};

#include "ADN_Connector_String.inl"

#endif // __ADN_Connector_String_h_