// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Table_h_
#define __Table_h_

#include "Table_ABC.h"
#include "ESRI.h"

namespace plugins
{
namespace crossbow
{
    class Database;
    class Row;
    class DatabaseEditor_ABC;

// =============================================================================
/** @class  Table
    @brief  Table
*/
// Created: SBO 2007-08-30
// =============================================================================
class Table : public Table_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Table( ITablePtr table, DatabaseEditor_ABC& editor );
    virtual ~Table();
    //@}

    //! @name Operations
    //@{
    virtual Row_ABC& CreateRow();
    virtual void DeleteRows( const std::string& query );
    virtual void InsertRow( const Row_ABC& row );
    virtual void UpdateRow( const Row_ABC& row );
    virtual void Clear();
    virtual Row_ABC* Find( const std::string& query, bool forceUpdate );
    virtual Row_ABC* GetNextRow();
    //@}


    //! @name Transactions
    //@{
    virtual void BeginTransaction();
    virtual void EndTransaction();
    //@}

protected:
    //! @name Helpers
    //@{
    DatabaseEditor_ABC& editor_;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Table( const Table& );            //!< Copy constructor
    Table& operator=( const Table& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{    
    ITablePtr table_;
    ICursorPtr cursor_;
    std::auto_ptr< crossbow::Row > row_;        
    //@}
};

}
}

#endif // __Table_h_
