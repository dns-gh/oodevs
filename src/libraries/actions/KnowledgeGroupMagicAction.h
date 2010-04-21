// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeGroupMagicAction_h_
#define __KnowledgeGroupMagicAction_h_

#include "ActionWithTarget_ABC.h"

namespace kernel
{
    class Controller;
    class MagicActionType;
    class ModelVisitor_ABC;
}

namespace actions
{

// =============================================================================
/** @class  KnowledgeGroupMagicAction
    @brief  KnowledgeGroupMagicAction
*/
// Created: JSR 2010-04-20
// =============================================================================
class KnowledgeGroupMagicAction : public ActionWithTarget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true );
             KnowledgeGroupMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity );
    virtual ~KnowledgeGroupMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

}

#endif // __KnowledgeGroupMagicAction_h_
