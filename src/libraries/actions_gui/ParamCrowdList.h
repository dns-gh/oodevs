// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamCrowdList_h_
#define __ParamCrowdList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Entity_ABC;
    class Population_ABC;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamCrowdList
    @brief  ParamCrowdList
*/
// Created: FPO 2011-05-30
// =============================================================================
class ParamCrowdList : public EntityListParameter< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    ParamCrowdList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamCrowdList();
    //@}

    //! @name Operations
    //@{
    void SetName( const QString& name );
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Population_ABC >* CreateElement( const kernel::Population_ABC& potential );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamCrowdList_h_
