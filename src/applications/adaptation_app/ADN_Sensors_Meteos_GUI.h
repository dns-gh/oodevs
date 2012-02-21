//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Meteos_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 4 $
// $Workfile: ADN_Sensors_Meteos_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Meteos_GUI_h_
#define __ADN_Sensors_Meteos_GUI_h_

#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_ModificatorTable.h"

//-----------------------------------------------------------------------------
// Internal Table connector
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_Meteos : public ADN_Connector_Table_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
    ADN_CT_Sensors_Meteos( ADN_Table& tab );
    //@}

    //! @name Operations
    //@{
    void AddSubItems(int i,void *obj);
    //@}
};

// =============================================================================
/** @class  ADN_Sensors_Meteos_GUI
    @brief  ADN_Sensors_Meteos_GUI
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_Meteos_GUI : public ADN_Sensors_ModificatorTable< ADN_CT_Sensors_Meteos >
{

public:
    explicit ADN_Sensors_Meteos_GUI(QWidget * parent = 0 );
    virtual ~ADN_Sensors_Meteos_GUI();

private:
    //! @name ADN_Sensors_MaterialsTable implementation
    //@{
    virtual void InternalEmit();
    //@}
};

#endif // __ADN_Sensors_Meteos_GUI_h_