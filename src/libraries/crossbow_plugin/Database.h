// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_Database_h_
#define __crossbow_Database_h_

#include "Database_ABC.h"
#include <map>

namespace boost 
{
    template<typename T> class shared_ptr;
}

namespace dispatcher
{
    class Config;
}

namespace plugins
{
namespace crossbow
{
    class QueryBuilder_ABC;
    class DatabaseEditor_ABC;

// =============================================================================
/** @class  Database
    @brief  Database
*/
// Created: SBO 2007-08-30
// =============================================================================
class Database : public Database_ABC
{
public:
    //! @name Constructors/Destructor
    //@{    
    virtual ~Database();
    //@}

    //! @name 
    //@{
    virtual Table_ABC&  OpenBufferedTable( const std::string& name, bool clear = true );
    virtual Table_ABC*  OpenTable( const std::string& name );
    virtual void        ClearTable( const std::string& name );
    virtual void ReleaseTable( const std::string& name );
    //@}

    //! @name 
    //@{
    virtual std::string GetTableName( const std::string& name ) const;
    virtual void Execute( const QueryBuilder_ABC& builder );
    //@}

    //! @name 
    //@{
    virtual void Flush();
    //@}

protected:
    //! @name Constructor
    //@{
    Database();
    //@}

    //! @name Workspace builder
    //@{
    void Initialize( IWorkspacePtr spWorkspace );
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    Database( const Database& );            //!< Copy constructor
    Database& operator=( const Database& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    Table_ABC* OpenWrappedTable( const std::string& name );
    bool       IsValid() const;
    //@}
    
    //! @name Types
    //@{
    typedef boost::shared_ptr< Table_ABC >      T_TablePtr;
    typedef std::map< std::string, T_TablePtr > T_Tables;
    typedef T_Tables::iterator                  IT_Tables;
    //@}

private:
    //! @name Member data
    //@{
    IWorkspacePtr        workspace_;
    IFeatureWorkspacePtr featureWorkspace_;
    IWorkspaceEditPtr    workspaceEdit_;
    T_Tables             openedTables_;
    std::auto_ptr< DatabaseEditor_ABC > editor_;
    //@}
};

}
}

#endif // __crossbow_Database_h_
