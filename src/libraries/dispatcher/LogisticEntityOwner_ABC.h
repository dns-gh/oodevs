// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticEntityOwner_ABC_h_
#define __LogisticEntityOwner_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ParentEntity;
}

namespace dispatcher
{

// =============================================================================
/** @class  LogisticEntityOwner_ABC
    @brief  LogisticEntityOwner_ABC
*/
// Created: NLD 2011-01-17
// =============================================================================
class LogisticEntityOwner_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticEntityOwner_ABC() {};
    virtual ~LogisticEntityOwner_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( sword::ParentEntity& msg ) const = 0;
    //@}
};

} // end namespace LogisticEntityOwner

#endif // __LogisticEntityOwner_ABC_h_
