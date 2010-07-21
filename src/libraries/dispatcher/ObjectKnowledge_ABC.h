// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dispatcher_ObjectKnowledge_ABC_h_
#define __dispatcher_ObjectKnowledge_ABC_h_

#include "Sendable.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

namespace kernel
{
    class ActionController;
}

namespace dispatcher
{

// =============================================================================
/** @class  ObjectKnowledge_ABC
    @brief  ObjectKnowledge_ABC
*/
// Created: PHC 2010-07-20
// =============================================================================
class ObjectKnowledge_ABC : public Sendable< kernel::ObjectKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledge_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::ObjectKnowledge_ABC >( id, name ) {}
    virtual ~ObjectKnowledge_ABC(){};
    //@}
};

}

#endif // __dispatcher_ObjectKnowledge_ABC_h_
