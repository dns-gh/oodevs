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


//*****************************************************************************
// Created: JDY 03-07-28
//*****************************************************************************
class ADN_ListView_Units
: public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_ListView_Units( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_ListView_Units();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    std::string GetToolTipFor( QListViewItem& item );
    //@}
};


#endif // __ADN_ListView_Units_h_
