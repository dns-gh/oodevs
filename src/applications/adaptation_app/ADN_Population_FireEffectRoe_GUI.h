//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_FireEffectRoe_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 4 $
// $Workfile: ADN_Population_FireEffectRoe_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Population_FireEffectRoe_GUI_h_
#define __ADN_Population_FireEffectRoe_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
class ADN_Population_FireEffectRoe_GUI : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_FireEffectRoe_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Population_FireEffectRoe_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Population_FireEffectRoe_GUI_h_