// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LogTable_h_
#define __LogTable_h_

#include "Table.h"

namespace geostore
{

// =============================================================================
/** @class  LogTable
    @brief  LogTable
*/
// Created: AME 2010-07-28
// =============================================================================
class LogTable : public Table
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogTable( sqlite3* db );
    virtual ~LogTable();
    //@}

    bool GetLastAccessTime( const std::string& layerName, std::time_t& time );
    void SetLastAccessTime( const std::string& layerName, const std::time_t& time );

private:
    void CreateStructure();
};

} //! namespace geostore

#endif // __LogTable_h_
