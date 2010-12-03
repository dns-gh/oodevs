// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_WeatherFireEffects_Table.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 3 $
// $Workfile: ADN_WeatherFireEffects_Table.h $
//
// *****************************************************************************

#ifndef __ADN_WeatherFireEffects_Table_h_
#define __ADN_WeatherFireEffects_Table_h_

#include "ADN_Table.h"
#include <qapplication.h>

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_WeatherFireEffects_Table
@par    Using example
@code
ADN_WeatherFireEffects_Table;
@endcode
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_WeatherFireEffects_Table
    : public ADN_Table2
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_WeatherFireEffects_Table( QWidget* pParent = 0 );
    virtual ~ADN_WeatherFireEffects_Table();
    //@}
};

#endif // __ADN_WeatherFireEffects_Table_h_
