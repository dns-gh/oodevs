// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamUrbanBlockList_h_
#define __ParamUrbanBlockList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Object_ABC;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamUrbanBlockList
    @brief  ParamUrbanBlockList
*/
// Created: FPO 2011-05-31
// =============================================================================
class ParamUrbanBlockList : public EntityListParameter< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamUrbanBlockList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~ParamUrbanBlockList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetName( const QString& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamUrbanBlockList( const ParamUrbanBlockList& );            //!< Copy constructor
    ParamUrbanBlockList& operator=( const ParamUrbanBlockList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::Object_ABC >* CreateElement( const kernel::Object_ABC& potential );
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

#endif // __ParamUrbanBlockList_h_
