//*****************************************************************************
//
// $Created: JDY 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Combo_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 15:47 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Combo_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Combo_ABC_h_
#define __ADN_Connector_Combo_ABC_h_

#include "ADN_Connector_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************

class ADN_Connector_Combo_ABC
: public ADN_Connector_Vector_ABC
{
public:
    explicit ADN_Connector_Combo_ABC(): ADN_Connector_Vector_ABC() {}
    virtual ~ADN_Connector_Combo_ABC(){}
    virtual void* SetNdxChanged(int ndx)=0;
    virtual bool IsConnected() const =0;

protected:
    virtual void  ClearPrivate(bool bInConnection=false)=0;
    virtual void  SetDataPrivate(void *data)=0;    
};

class ADN_ComboBoxItem; 


template < class T >
class ADN_Connector_Combo
: public ADN_Connector_Combo_ABC
{
public:
    explicit ADN_Connector_Combo(T* combo);
    virtual ~ADN_Connector_Combo();

    bool IsConnected() const;
           
protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual typename T::T_ItemType* CreateItem(void * obj) = 0;
    virtual bool  AddItemPrivate(void *obj,bool bCreateCommand=false);
    virtual bool  RemItemPrivate(void *item,bool bCreateCommand=false);
    virtual void  ClearPrivate(bool bInConnection=false);
    virtual void  SetDataPrivate(void *data);    
    virtual void* SetNdxChanged(int ndx);
    
protected:
    T*                 pCombo_;
    bool               bIsConnected_;
};

#include "ADN_Connector_Combo_ABC.inl"


#endif // __ADN_Connector_Combo_ABC_h_