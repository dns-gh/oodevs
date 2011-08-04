//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Bool.h $
// $Author: Ape $
// $Modtime: 28/02/05 17:27 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Bool.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Bool_h_
#define __ADN_Connector_Bool_h_

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
template <class T>
class ADN_Connector_Bool
: public ADN_Connector_ABC
{
public:
    explicit ADN_Connector_Bool(T* gfx);
    virtual ~ADN_Connector_Bool();

    void SetDataChanged(bool b);
    bool IsConnected() const;

protected:
    void SetDataPrivate( void *data );

private:
    void connectNotify(const char *signal);
    void disconnectNotify(const char *signal);

private:
    T*  pGfx_;
    bool bIsConnected_;
};

#include "ADN_Connector_Bool.inl"


#endif // __ADN_Connector_Bool_h_