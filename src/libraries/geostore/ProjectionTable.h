// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProjectionTable_h_
#define __ProjectionTable_h_

#include "Table.h"

namespace geostore
{
// =============================================================================
/** @class  ProjectionTable
    @brief  ProjectionTable
*/
// Created: AME 2010-08-02
// =============================================================================
class ProjectionTable : private Table
{
public:
    explicit ProjectionTable( sqlite3* db );
};

}

#endif // __ProjectionTable_h_
