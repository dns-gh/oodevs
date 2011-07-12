// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_Database_ABC_h_
#define __crossbow_Database_ABC_h_

namespace plugins
{
namespace crossbow
{
    class Table_ABC;

// =============================================================================
/** @class  Database_ABC
    @brief  Database_ABC
*/
// Created: SBO 2007-08-30
// =============================================================================
class Database_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Database_ABC() {}
    virtual ~Database_ABC() {}
    //@}

    //! @name
    //@{
    virtual std::string GetTableName( const std::string& name ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual Table_ABC&  OpenBufferedTable( const std::string& name, bool clear = true ) = 0;
    virtual Table_ABC*  OpenTable( const std::string& name ) = 0;
    virtual void        ClearTable( const std::string& name, const std::string& filter ) = 0;
    virtual void        Flush() = 0;
    //@}
};

}
}

#endif // __Database_ABC_h_
