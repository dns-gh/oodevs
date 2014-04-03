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

#include "Action_ABC.h"

namespace kernel
{
    class Controller;
    class MagicActionType;
}

namespace actions
{

// =============================================================================
/** @class  KnowledgeGroupMagicAction
    @brief  KnowledgeGroupMagicAction
*/
// Created: JSR 2010-04-20
// =============================================================================
class KnowledgeGroupMagicAction : public Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true );
    virtual ~KnowledgeGroupMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
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
