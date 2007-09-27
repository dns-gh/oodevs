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

namespace crossbow
{
    class Database;
    class Row;

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
             Table( ITablePtr table, const std::string& name );
    virtual ~Table();
    //@}

    //! @name Operations
    //@{
    virtual Row_ABC& CreateRow();
    virtual void DeleteRows( const std::string& query );
    virtual void UpdateRow( const Row_ABC& row );
    virtual void Clear();
    virtual Row_ABC* Find( const std::string& query );
    virtual Row_ABC* GetNextRow();

    virtual void BeginTransaction();
    virtual void EndTransaction();
    //@}

protected:
    //! @name Helpers
    //@{
    bool InTransaction() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Table( const Table& );            //!< Copy constructor
    Table& operator=( const Table& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    ITablePtr table_;
    ICursorPtr cursor_;
    std::auto_ptr< crossbow::Row > row_;
    bool inTransaction_;
    //@}
};

}

#endif // __Table_h_
