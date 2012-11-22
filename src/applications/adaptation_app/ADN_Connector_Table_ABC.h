//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Table_ABC.h $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 5 $
// $Workfile: ADN_Connector_Table_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Connector_Table_ABC_h_
#define __ADN_Connector_Table_ABC_h_

#include "ADN_Table.h"
#include "ADN_Connector_Vector_ABC.h"
#include <boost/noncopyable.hpp>

//*****************************************************************************
// Created: JDY 03-07-09
//*****************************************************************************
class ADN_Connector_Table_ABC : public ADN_Connector_Vector_ABC
                               , private boost::noncopyable
{

public:
             ADN_Connector_Table_ABC( ADN_Table& tab, bool bWithSort );
    virtual ~ADN_Connector_Table_ABC();

    virtual bool LessComparison( void* pL, void* pR ) const;

    bool IsConnected() const;

protected:
    virtual void AddSubItems( int nRow, void* pObj );

private:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual bool AddItemPrivate( void* item );
    virtual bool RemItemPrivate( void* item );

    void ClearPrivate(bool bInConnection=false);
    void SetDataPrivate(void *data);

protected:
    std::vector<void*> vDatas_;
    ADN_Table& tab_;

    bool bIsConnected_;
    bool bWithSort_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::IsConnected
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
inline
bool ADN_Connector_Table_ABC::IsConnected() const
{
    return bIsConnected_;
}

#endif // __ADN_Connector_Table_ABC_h_