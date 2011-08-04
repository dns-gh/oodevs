//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_ListView_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 6 $
// $Workfile: ADN_Connector_ListView_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_ListView_ABC_h_
#define __ADN_Connector_ListView_ABC_h_

#include "ADN_ListView.h"
#include "ADN_ListViewItem.h"
#include "ADN_Connector_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-09
//*****************************************************************************
class ADN_Connector_ListView_ABC
: public ADN_Connector_Vector_ABC
{
public:
    explicit ADN_Connector_ListView_ABC(ADN_ListView& list, const char* szName = 0 );
    virtual ~ADN_Connector_ListView_ABC();

    bool IsConnected() const;

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual void SetDataPrivate(void *data);
    virtual bool AddItemPrivate(void *obj,bool bCreateCommand = false );
    virtual bool RemItemPrivate(void *item, bool bCreateCommand = false );
    virtual void SwapItemPrivate(int i,int j);
    virtual void ClearPrivate(bool bInConnection=false);

    virtual ADN_ListViewItem* CreateItem(void * obj) = 0;

private:
    ADN_Connector_ListView_ABC& operator=( const ADN_Connector_ListView_ABC& );

protected:
    ADN_ListView& list_;
    bool bSwap_;
    bool bIsConnected_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::IsConnected
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
inline
bool ADN_Connector_ListView_ABC::IsConnected() const
{
    return bIsConnected_;
}


#endif // __ADN_Connector_ListView_ABC_h_