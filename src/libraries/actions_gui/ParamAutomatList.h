// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomatList_h_
#define __ParamAutomatList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Automat_ABC;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamAutomatList
    @brief  ParamAutomatList
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomatList : public EntityListParameter< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomatList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamAutomatList();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Automat_ABC >* CreateElement( const kernel::Automat_ABC& potential );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamAutomatList_h_
