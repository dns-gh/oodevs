//*****************************************************************************
//
// $Created: JDY 03-09-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Postures_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 4 $
// $Workfile: ADN_Equipement_Postures_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Equipement_Postures_GUI_h_
#define __ADN_Equipement_Postures_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-09-29
//*****************************************************************************
class ADN_Equipement_Postures_GUI : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipement_Postures_GUI( const QString& objectName, const QString& strColCaption, ADN_Connector_ABC*& connector,  QWidget* pParent = 0 );
    virtual ~ADN_Equipement_Postures_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Equipement_Postures_GUI_h_