// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensor_Wizard.h $
// $Author: Ape $
// $Modtime: 28/01/05 15:29 $
// $Revision: 4 $
// $Workfile: ADN_Sensor_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Sensor_Wizard_h_
#define __ADN_Sensor_Wizard_h_

#include "ADN_Sensors_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Sensors_Data::SensorInfos> ADN_Wizard_ABC_ADN_Sensors_Data_SensorInfos_;
// =============================================================================
/** @class  ADN_Sensor_Wizard
    @brief  ADN_Sensor_Wizard
    @par    Using example
    @code
    ADN_Sensor_Wizard;
    @endcode
*/
// Created: AGN 2003-11-03
// =============================================================================
class ADN_Sensor_Wizard
: public ADN_Wizard_ABC_ADN_Sensors_Data_SensorInfos_
{
    MT_COPYNOTALLOWED( ADN_Sensor_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Sensor_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Sensor_Wizard();
    //@}
};


#endif // __ADN_Sensor_Wizard_h_
