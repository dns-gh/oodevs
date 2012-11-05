//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Int.h $
// $Author: Ape $
// $Modtime: 16/12/04 10:12 $
// $Revision: 2 $
// $Workfile: ADN_Connector_Int.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Int_h_
#define __ADN_Connector_Int_h_

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
template <class T>
class ADN_Connector_Int : public ADN_Connector_ABC
{
public:
    explicit ADN_Connector_Int(T* gfx);
    virtual ~ADN_Connector_Int();

    void  SetDataChanged(const QString& string);
    bool IsConnected() const;

protected:
    void  SetDataPrivate(void *data);

private:
    void  connectNotify(const char *signal);
    void  disconnectNotify(const char *signal);

private:
    T*  pGfx_;
    bool bIsConnected_;
};

#include "ADN_Connector_Int.inl"

#endif // __ADN_Connector_Int_h_