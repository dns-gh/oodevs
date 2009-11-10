// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConnectionDatabase_h_
#define __ConnectionDatabase_h_

#include "Database.h"

namespace plugins
{
namespace crossbow
{
    class Table_ABC;

// =============================================================================
/** @class  ConnectionDatabase
    @brief  ConnectionDatabase
*/
// Created: JCR 2009-02-03
// =============================================================================
class ConnectionDatabase : public Database
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ConnectionDatabase( const std::string& url );
    virtual ~ConnectionDatabase();
    //@}

    //! @name 
    //@{
    virtual std::string GetTableName( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectionDatabase( const ConnectionDatabase& );            //!< Copy constructor
    ConnectionDatabase& operator=( const ConnectionDatabase& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    IPropertySetPtr CreateProperty( const std::string& url );
    //@}

private:
    //! @name Attributes
    //@{
    std::string database_;
    std::string protocol_;
    std::string schema_;
    //@}
};

}
}

#endif // __ConnectionDatabase_h_
