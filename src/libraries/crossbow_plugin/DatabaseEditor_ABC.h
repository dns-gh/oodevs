// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __plugin_crossbow_DatabaseEditor_ABC_h_
#define __plugin_crossbow_DatabaseEditor_ABC_h_

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  DatabaseEditor_ABC
    @brief  DatabaseEditor_ABC
*/
// Created: JCR 2009-04-22
// =============================================================================
class DatabaseEditor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            DatabaseEditor_ABC() {}
    virtual ~DatabaseEditor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Lock() = 0;
    virtual void UnLock() = 0;
    virtual void StartEdit() = 0;
    virtual void StopEdit() = 0;
    //@}

    //! @name Transactions
    //@{
    virtual bool InTransaction() const = 0;
    virtual void BeginTransaction() = 0;
    virtual void EndTransaction() = 0;
    //@}
};

} // end namespace crossbow
} // end namespace plugin

#endif // __plugin_crossbow_DatabaseEditor_ABC_h_
