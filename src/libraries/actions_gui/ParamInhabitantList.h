// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamInhabitantList_h_
#define __ParamInhabitantList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Entity_ABC;
    class Inhabitant_ABC;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamInhabitantList
    @brief  ParamInhabitantList
*/
// Created: FPO 2011-05-30
// =============================================================================
class ParamInhabitantList : public EntityListParameter< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamInhabitantList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamInhabitantList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetName( const QString& name );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Inhabitant_ABC >* CreateElement( const kernel::Inhabitant_ABC& potential );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamInhabitantList_h_
