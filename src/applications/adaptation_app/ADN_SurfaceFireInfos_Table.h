// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_SurfaceFireInfos_Table_h_
#define __ADN_SurfaceFireInfos_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_SurfaceFireInfos_Table
@brief  ADN_SurfaceFireInfos_Table
*/
// Created: JSR 2010-12-03
// =============================================================================
class ADN_SurfaceFireInfos_Table : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_SurfaceFireInfos_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_SurfaceFireInfos_Table();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_SurfaceFireInfos_Table_h_
