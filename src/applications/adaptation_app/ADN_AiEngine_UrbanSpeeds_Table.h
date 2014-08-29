// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_AiEngine_UrbanSpeeds_Table_h_
#define __ADN_AiEngine_UrbanSpeeds_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_AiEngine_UrbanSpeeds_Table
    @brief  ADN_AiEngine_UrbanSpeeds_Table
*/
// Created: JSR 2014-07-09
// =============================================================================
class ADN_AiEngine_UrbanSpeeds_Table : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructor / Destructor
    //@{
    explicit ADN_AiEngine_UrbanSpeeds_Table( const QString& objectName, ADN_Ref_ABC& vector, QWidget * parent = 0 );
    virtual ~ADN_AiEngine_UrbanSpeeds_Table();
    //@}

    void AddRow( int row, void* data );
};

#endif // __ADN_AiEngine_UrbanSpeeds_Table_h_
