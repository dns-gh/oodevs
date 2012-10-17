// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ACTION_H
#define SWORD_ACTION_H

#include "Entities/Actions/PHY_Action_ABC.h"
#include <string>

class MIL_Entity_ABC;
class DEC_Decision_ABC;

namespace core
{
    class Model;
}

namespace sword
{
    class Sink;

// =============================================================================
/** @class  ModelCounter
    @brief  ModelCounter
*/
// Created: MCO 2012-10-10
// =============================================================================
class Action : public PHY_Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Action( Sink& sink, MIL_Entity_ABC& entity, const std::string& name, core::Model& parameters );
    virtual ~Action();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Suspend();
    virtual void Resume();
    virtual void Stop();
    virtual void Execute();
    virtual void ExecuteSuspended();
    //@}

    //! @name Member data
    //@{
    Sink& sink_;
    DEC_Decision_ABC& decision_;
    std::size_t command_;
    //@}
};

}

#endif // SWORD_ACTION_H
