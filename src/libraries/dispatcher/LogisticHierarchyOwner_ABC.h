// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticHierarchyOwner_ABC_h_
#define __LogisticHierarchyOwner_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword 
{
    class ParentEntity;
}

namespace dispatcher 
{
// =============================================================================
/** @class  LogisticHierarchyOwner_ABC
    @brief  LogisticHierarchyOwner_ABC
*/
// Created: NLD 2011-01-17
// =============================================================================
class LogisticHierarchyOwner_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchyOwner_ABC() {};
    virtual ~LogisticHierarchyOwner_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( sword::ParentEntity& msg ) const = 0;
    //@}
};

} 

#endif // __LogisticHierarchyOwner_ABC_h_
