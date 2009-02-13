// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Table_ABC_h_
#define __Table_ABC_h_

namespace plugins
{
namespace crossbow
{
    class Row_ABC;

// =============================================================================
/** @class  Table_ABC
    @brief  Table_ABC
*/
// Created: SBO 2007-08-30
// =============================================================================
class Table_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Table_ABC() {}
    virtual ~Table_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Row_ABC& CreateRow() = 0;
    virtual void DeleteRows( const std::string& query ) = 0;
    virtual void UpdateRow( const Row_ABC& row ) = 0;
    virtual void Clear() = 0;

    virtual Row_ABC* Find( const std::string& query ) = 0;    
    virtual Row_ABC* GetNextRow() = 0;

    virtual void BeginTransaction() = 0;
    virtual void EndTransaction() = 0;
    //@}
};

}
}

#endif // __Table_ABC_h_
