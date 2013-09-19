//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Units.h $
// $Author: Ape $
// $Modtime: 25/04/05 11:14 $
// $Revision: 4 $
// $Workfile: ADN_ListView_Units.h $
//
//*****************************************************************************

#ifndef __ADN_ListView_Units_h_
#define __ADN_ListView_Units_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"
#include "ADN_Units_Data.h"

//*****************************************************************************
// Created: JDY 03-07-28
//*****************************************************************************
class ADN_ListView_Units
    : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_Units( QWidget* pParent );
    virtual ~ADN_ListView_Units();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ListView_Units_h_
