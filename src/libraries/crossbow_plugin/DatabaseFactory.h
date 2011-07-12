// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_DatabaseFactory_h_
#define __crossbow_DatabaseFactory_h_


namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
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
    std::auto_ptr< crossbow::Database_ABC > Create( const std::string& path, const std::string& name, dispatcher::Logger_ABC& logger ) const;
    //@}

private:
    //! @name Operations
    //@{
    std::auto_ptr< crossbow::Database_ABC > CreateShapefile( const std::string& path, const std::string& name ) const;
    std::auto_ptr< crossbow::Database_ABC > CreatePgeo( const std::string& path, const std::string& name ) const;
    std::auto_ptr< crossbow::Database_ABC > CreatePostgreSQL( const std::string& name, dispatcher::Logger_ABC& logger ) const;
    std::auto_ptr< crossbow::Database_ABC > CreateSDE( const std::string& name, dispatcher::Logger_ABC& logger ) const;
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

#endif // __crossbow_DatabaseFactory_h_
