// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_AviationResourceQuotas_GUI_h_
#define __ADN_Equipments_AviationResourceQuotas_GUI_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_Equipments_AviationResourceQuotas_GUI
    @brief  ADN_Equipments_AviationResourceQuotas_GUI
*/
// Created: JSR 2013-10-16
// =============================================================================
class ADN_Equipments_AviationResourceQuotas_GUI : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_AviationResourceQuotas_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_AviationResourceQuotas_GUI();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Equipments_AviationResourceQuotas_GUI_h_
