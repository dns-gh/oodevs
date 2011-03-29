// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_ActionSerializer_ABC_h_
#define __crossbow_ActionSerializer_ABC_h_


namespace kernel
{
    class Entity_ABC;
}

namespace actions
{
    class Action_ABC;
}

namespace dispatcher
{
    class Agent_ABC;
    class Automat_ABC;
}

namespace plugins
{
namespace crossbow
{

    class Row_ABC;

// =============================================================================
/** @class  ActionSerializer_ABC
    @brief  ActionSerializer_ABC
*/
// Created: JCR 2010-12-07
// =============================================================================
class ActionSerializer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            ActionSerializer_ABC() {}
    virtual ~ActionSerializer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SerializeCreation( const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action ) const = 0;
    //@}
};

}
}

#endif // __crossbow_ActionSerializer_ABC_h_
