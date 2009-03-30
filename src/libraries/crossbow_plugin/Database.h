// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Database_h_
#define __Database_h_

#include "Database_ABC.h"
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

    //! @name Operations
    //@{
    virtual void Lock();
    virtual void UnLock();
    virtual void StartEdit();
    virtual void StopEdit();
    //@}

    //! @name 
    //@{
    virtual Table_ABC&  OpenBufferedTable( const std::string& name, bool clear = true );
    virtual Table_ABC*  OpenTable( const std::string& name );
    virtual void        ClearTable( const std::string& name );
    virtual void ReleaseTable( const std::string& name );
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
    //@}
    

    //! @name Types
    //@{
    typedef std::map< std::string, Table_ABC* > T_Tables;
    typedef T_Tables::iterator                  IT_Tables;
    //@}

private:
    //! @name Member data
    //@{
    IFeatureWorkspacePtr workspace_;
    IWorkspaceEditPtr    workspaceEdit_;
    T_Tables             openedTables_;
    //@}
};

}
}

#endif // __Database_h_
