//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Enum.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Enum.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Enum_h_
#define __ADN_Connector_Enum_h_

#include "ADN_Connector_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-22
//*****************************************************************************
template <class T>
class ADN_Connector_Enum
:   public ADN_Connector_Vector_ABC
{
public:
    explicit ADN_Connector_Enum(T* pGfx);
    virtual ~ADN_Connector_Enum();
    virtual void* SetNdxChanged(int ndx);

    bool IsConnected() const;

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual bool AddItemPrivate(void *obj,bool bInConnection=false);
    virtual bool RemItemPrivate(void *item);
    virtual void ClearPrivate(bool bInConnection=false);
    virtual void SetDataPrivate(void *data);
    
    virtual std::string GetItem(void * obj)=0;

protected:
    T*                  pGfx_;
    int                 nNb_;
    bool bIsConnected_;
};

#include "ADN_Connector_Enum.inl"

#endif // __ADN_Connector_Enum_h_