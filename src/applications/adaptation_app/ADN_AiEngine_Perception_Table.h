// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_AiEngine_Perception_Table_h_
#define __ADN_AiEngine_Perception_Table_h_

#include "ADN_Table_ABC.h"

class ADN_Ref_ABC;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_AiEngine_Perception_Table
    @brief  ADN_AiEngine_Perception_Table
*/
// Created: JSR 2014-07-04
// =============================================================================
class ADN_AiEngine_Perception_Table : public QTableWidget, public ADN_Table_ABC
{
    Q_OBJECT;

public:
             ADN_AiEngine_Perception_Table( const QString& objectName, ADN_Ref_ABC& vector, QWidget* parent = 0 );
    virtual ~ADN_AiEngine_Perception_Table();

private:
    virtual void SetEnabled( bool enable );
    virtual void setNumRows( int rows );
    virtual void RemoveItem( void* item );
    virtual void AddRow( int nRow, void* pObj );
};

#endif // __ADN_AiEngine_Perception_Table_h_
