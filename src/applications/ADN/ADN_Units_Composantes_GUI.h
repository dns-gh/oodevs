//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Composantes_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:59 $
// $Revision: 5 $
// $Workfile: ADN_Units_Composantes_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Units_Composantes_GUI_h_
#define __ADN_Units_Composantes_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-07-28
//*****************************************************************************
class ADN_Units_Composantes_GUI
: public ADN_Table2
{
    Q_OBJECT
public:
    explicit ADN_Units_Composantes_GUI(QWidget * parent = 0 );
    virtual ~ADN_Units_Composantes_GUI();

    void AddNewElement( int n );
    void RemoveCurrentElement();

    virtual void OnContextMenu(int row,int col,const QPoint& pt);

private slots:
    void MenuListItemSelected();

private:
    bool bMenuListItemSelected_;
};


#endif // __ADN_Units_Composantes_GUI_h_