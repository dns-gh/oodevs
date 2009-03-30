// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FileDatabase_h_
#define __FileDatabase_h_

#include "Database.h"
#include "ESRI.h"

namespace dispatcher
{
    class Config;
}

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  FileDatabase
    @brief  FileDatabase
*/
// Created: SBO 2007-08-30
// =============================================================================
class FileDatabase : public Database
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FileDatabase( const std::string& geodatabase );
    virtual ~FileDatabase();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FileDatabase( const FileDatabase& );            //!< Copy constructor
    FileDatabase& operator=( const FileDatabase& ); //!< Assignment operator
    //@}
};

}
}

#endif // __FileDatabase_h_
