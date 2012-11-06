// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Connector_Combo_h_
#define __ADN_Connector_Combo_h_

#include "ADN_Connector_Combo_ABC.h"
#include "ADN_Connector_Vector_ABC.h"

// =============================================================================
/** @class  ADN_Connector_Combo
    @brief  ADN_Connector_Combo
*/
// Created: ABR 2012-10-29
// =============================================================================
template< class T >
class ADN_Connector_Combo : public ADN_Connector_Combo_ABC
                          , public ADN_Connector_Vector_ABC

{
public:
    explicit ADN_Connector_Combo(T* combo);
    virtual ~ADN_Connector_Combo();

    bool IsConnected() const;

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual typename T::T_ItemType* CreateItem(void * obj) = 0;
    virtual bool  AddItemPrivate( void* obj );
    virtual bool  RemItemPrivate( void* item );
    virtual void  ClearPrivate(bool bInConnection=false);
    virtual void  SetDataPrivate(void *data);
    virtual void* SetNdxChanged(int ndx);

protected:
    T*                 pCombo_;
    bool               bIsConnected_;
};

#include "ADN_Connector_Combo.inl"

#endif // __ADN_Connector_Combo_h_
