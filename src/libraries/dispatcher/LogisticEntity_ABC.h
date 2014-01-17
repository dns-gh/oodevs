// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DISPATCHER_LOGISTIC_ENTITY_ABC_H__
#define __DISPATCHER_LOGISTIC_ENTITY_ABC_H__

#include "LogisticHierarchyOwner_ABC.h"

namespace sword
{
    class ParentEntity;
}

namespace dispatcher
{
    class Automat_ABC;
    class Formation_ABC;
    class LogisticEntityOwner_ABC;

// =============================================================================
/** @class  LogisticEntity_ABC
    @brief  LogisticEntity_ABC
*/
// Created: AHC 2010-10
// =============================================================================
class LogisticEntity_ABC
{
public:
    //! @name Constructor/destructor
    //@{
             LogisticEntity_ABC() {};
    virtual ~LogisticEntity_ABC() {};
    //@}

    //! @name Network
    //@{
    virtual void Send( sword::ParentEntity& msg ) const = 0;
    //@}
};

} // namespace dispatcher

#endif // __DISPATCHER_LOGISTIC_ENTITY_ABC_H__

