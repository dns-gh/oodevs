// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Table_Objects_FirePropagationModifier_h_
#define __ADN_Table_Objects_FirePropagationModifier_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_Table_Objects_FirePropagationModifier
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_Table_Objects_FirePropagationModifier
    : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Table_Objects_FirePropagationModifier( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Table_Objects_FirePropagationModifier();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Table_Objects_FirePropagationModifier_h_
