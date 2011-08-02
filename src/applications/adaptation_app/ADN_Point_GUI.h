//*****************************************************************************
//
// $Created: AGN 03-07-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Point_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 6 $
// $Workfile: ADN_Point_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Point_GUI_h_
#define __ADN_Point_GUI_h_

#include "ADN_Table.h"
#include "ADN_Enums.h"
#include <QtGui/qapplication.h>


//*****************************************************************************
// Created: AGN 03-07-30
//*****************************************************************************
class ADN_Point_GUI
: public ADN_Table2
{
public:
    explicit ADN_Point_GUI( QWidget* pParent = 0 );
    virtual ~ADN_Point_GUI();

    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );

    void AddNewElement( E_KeyPoint nKeyPoint );
    void DeleteCurrentElement();

    bool Contains( E_KeyPoint nKeyPoint );
};


#endif // __ADN_Point_GUI_h_