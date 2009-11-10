// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __plugin_crossbow_DatabaseEditor_h_
#define __plugin_crossbow_DatabaseEditor_h_

#include "ESRI.h"
#include "DatabaseEditor_ABC.h"

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  DatabaseEditor
    @brief  DatabaseEditor
*/
// Created: JCR 2009-04-22
// =============================================================================
class DatabaseEditor : public DatabaseEditor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DatabaseEditor( IWorkspacePtr spWorkspace );
    virtual ~DatabaseEditor();
    //@}

    //! @name Operations
    //@{
    void Lock();
    void UnLock();
    void StartEdit();
    void StopEdit();    
    //@}

    //! @name 
    //@{
    bool InTransaction() const;
    void BeginTransaction();
    void EndTransaction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseEditor( const DatabaseEditor& );            //!< Copy constructor
    DatabaseEditor& operator=( const DatabaseEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsEditing() const;
    //@}

private:
    //! @name Member data
    //@{
    IWorkspaceEditPtr  workspaceEdit_;
    bool               inTransaction_;
    bool               editing_;
    //@}
};

}
}

#endif // __plugin_crossbow_DatabaseEditor_h_
