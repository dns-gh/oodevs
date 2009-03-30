// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_esri_DatabaseFactory_h_
#define __dispatcher_esri_DatabaseFactory_h_

namespace dispatcher
{
    class Config;
}

namespace plugins
{
    class Config;

namespace crossbow
{
    class Database_ABC;

// =============================================================================
/** @class  DatabaseFactory
    @brief  DatabaseFactory
*/
// Created: JCR 2009-02-10
// =============================================================================
class DatabaseFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             DatabaseFactory();
    virtual ~DatabaseFactory();
    //@}

    //! @name Operations
    //@{
    Database_ABC& Create( const dispatcher::Config& config, const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseFactory( const DatabaseFactory& );            //!< Copy constructor
    DatabaseFactory& operator=( const DatabaseFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __dispatcher_esri_DatabaseFactory_h_
