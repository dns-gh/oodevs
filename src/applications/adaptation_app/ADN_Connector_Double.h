//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Double.h $
// $Author: Ape $
// $Modtime: 16/12/04 10:12 $
// $Revision: 2 $
// $Workfile: ADN_Connector_Double.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Double_h_
#define __ADN_Connector_Double_h_

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
template <class T>
class ADN_Connector_Double
: public ADN_Connector_ABC
{
public:
    explicit ADN_Connector_Double(T* gfx);
    virtual ~ADN_Connector_Double();

    void  SetDataChanged(const QString& string);
    bool IsConnected() const;

protected:
    void  SetDataPrivate(void *data);
    virtual void ConnectPrivateSub( ADN_Connector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_ABC* pTarget );

private:
    void  connectNotify(const char *signal);
    void  disconnectNotify(const char *signal);

private:
    bool bIsConnected_;
    T*  pGfx_;
};

#include "ADN_Connector_Double.inl"

#endif // __ADN_Connector_Double_h_