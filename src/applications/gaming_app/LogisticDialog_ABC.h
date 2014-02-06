// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LogisticDialog_ABC_h_
#define __LogisticDialog_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  LogisticDialog_ABC
    @brief  LogisticDialog_ABC
*/
// Created: LGY 2013-12-11
// =============================================================================
class LogisticDialog_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
              LogisticDialog_ABC() {}
     virtual ~LogisticDialog_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Purge() = 0;
    virtual void FillCurrentModel( const kernel::Entity_ABC& entity ) = 0;
    virtual void FillHistoryModel() = 0;
    virtual bool IsVisible() const = 0;
    //@}
};

#endif // LogisticDialog_ABC
