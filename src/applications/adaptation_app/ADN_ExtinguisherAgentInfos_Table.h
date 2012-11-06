// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ExtinguisherAgentInfos_Table_h_
#define __ADN_ExtinguisherAgentInfos_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_ExtinguisherAgentInfos_Table
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_ExtinguisherAgentInfos_Table : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ExtinguisherAgentInfos_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_ExtinguisherAgentInfos_Table();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_ExtinguisherAgentInfos_Table_h_
