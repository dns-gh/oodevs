//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Environments_GUI.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:51 $
// $Revision: 4 $
// $Workfile: ADN_Sensors_Environments_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Environments_GUI_h_
#define __ADN_Sensors_Environments_GUI_h_

#include "ADN_Sensors_ModificatorTable.h"

// =============================================================================
/** @class  ADN_Sensors_Environments_GUI
    @brief  ADN_Sensors_Environments_GUI
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_Environments_GUI : public ADN_Sensors_ModificatorTable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             ADN_Sensors_Environments_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Sensors_Environments_GUI();
    //@}

private:
    //! @name ADN_Sensors_MaterialsTable implementation
    //@{
    virtual void InternalEmit( const QModelIndex& current );
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Sensors_Environments_GUI_h_