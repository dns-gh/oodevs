// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __dispatcher_UrbanKnowledge_ABC_h_
#define __dispatcher_UrbanKnowledge_ABC_h_

#include "Sendable.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"

namespace kernel
{
    class ActionController;
}

namespace dispatcher
{

// =============================================================================
/** @class  UrbanKnowledge_ABC
    @brief  UrbanKnowledge_ABC
*/
// Created: PHC 2010-07-20
// =============================================================================
class UrbanKnowledge_ABC : public Sendable< kernel::UrbanKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanKnowledge_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::UrbanKnowledge_ABC >( id, name ) {}
    virtual ~UrbanKnowledge_ABC(){};
    //@}
};

}

#endif // __dispatcher_UrbanKnowledge_ABC_h_
