// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_WeatherFireEffects_Table_h_
#define __ADN_WeatherFireEffects_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_WeatherFireEffects_Table
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_WeatherFireEffects_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_WeatherFireEffects_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_WeatherFireEffects_Table();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_WeatherFireEffects_Table_h_
