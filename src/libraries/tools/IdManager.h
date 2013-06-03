// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_IdManager_h_
#define __kernel_IdManager_h_

#include <boost/noncopyable.hpp>
#include <set>

namespace tools
{

// =============================================================================
/** @class  IdManager
    @brief  IdManager
*/
// Created: SBO 2006-09-26
// =============================================================================
class IdManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IdManager( bool bKeepIds = false );
    virtual ~IdManager();
    //@}

    //! @name Operations
    //@{
    unsigned long GetNextId( bool bSkipKeepId = false );
    void Reset();
    void Lock( unsigned long id, bool bSkipKeepId = false );
    void SetKeepIds( bool bKeepIds );
    bool HasBeenKept( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long max_;
    bool bKeepIds_;
    std::set< unsigned long > ids_;
    //@}
};

} //! namespace kernel

#endif // __kernel_IdManager_h_
