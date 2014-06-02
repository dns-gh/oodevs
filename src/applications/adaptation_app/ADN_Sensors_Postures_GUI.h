// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Sensors_Postures_GUI_h_
#define __ADN_Sensors_Postures_GUI_h_

#include "ADN_Sensors_ModificatorTable.h"

// =============================================================================
/** @class  ADN_Sensors_Postures_GUI
    @brief  ADN_Sensors_Postures_GUI
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_Postures_GUI : public ADN_Sensors_ModificatorTable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             ADN_Sensors_Postures_GUI( const QString& strColCaption, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Sensors_Postures_GUI();
    //@}

private:
    //! @name ADN_Sensors_MaterialsTable implementation
    //@{
    virtual void InternalEmit( const QModelIndex& current );
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Sensors_Postures_GUI_h_
