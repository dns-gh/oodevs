// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_MissionParameterValues_Table_h_
#define __ADN_MissionParameterValues_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_MissionParameterValues_Table
    @brief  ADN_MissionParameterValues_Table
*/
// Created: SBO 2006-12-05
// =============================================================================
class ADN_MissionParameterValues_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameterValues_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_MissionParameterValues_Table();
    //@}

private:
    //! @name Helpers
    //@{
    void AddNewElement();
    void RemoveCurrentElement();
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_MissionParameterValues_Table_h_
