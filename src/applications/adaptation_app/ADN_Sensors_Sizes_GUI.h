//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Sizes_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 4 $
// $Workfile: ADN_Sensors_Sizes_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Sizes_GUI_h_
#define __ADN_Sensors_Sizes_GUI_h_

#include "ADN_Sensors_ModificatorTable.h"

// =============================================================================
/** @class  ADN_Sensors_Sizes_GUI
    @brief  ADN_Sensors_Sizes_GUI
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_Sizes_GUI : public ADN_Sensors_ModificatorTable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             ADN_Sensors_Sizes_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Sensors_Sizes_GUI();
    //@}

private:
    //! @name ADN_Sensors_MaterialsTable implementation
    //@{
    virtual void InternalEmit();
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Sensors_Sizes_GUI_h_