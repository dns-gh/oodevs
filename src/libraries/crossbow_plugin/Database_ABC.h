// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Database_ABC_h_
#define __Database_ABC_h_

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

    //! @name Operations
    //@{
    virtual void Lock() = 0;
    virtual void UnLock() = 0;
    virtual Table_ABC* OpenTable( const std::string& name ) = 0;
    //@}
};

}

#endif // __Database_ABC_h_
