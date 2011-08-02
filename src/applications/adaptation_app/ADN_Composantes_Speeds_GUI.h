//*****************************************************************************
//
// $Created: JDY 03-07-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Speeds_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 4 $
// $Workfile: ADN_Composantes_Speeds_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_Speeds_GUI_h_
#define __ADN_Composantes_Speeds_GUI_h_

#include "ADN_CommonGfx.h"

#include <QtGui/qapplication.h>

//*****************************************************************************
// Created: JDY 03-07-15
//*****************************************************************************
class ADN_Composantes_Speeds_GUI
: public ADN_Table2
{
public:
    explicit ADN_Composantes_Speeds_GUI(QWidget * parent = 0 );
    virtual ~ADN_Composantes_Speeds_GUI();
};


#endif // __ADN_Composantes_Speeds_GUI_h_