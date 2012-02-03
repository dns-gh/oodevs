// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySearchItem_h_
#define __EntitySearchItem_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class ActionController;
}

namespace gui
{

// =============================================================================
/** @class  EntitySearchItem
    @brief  EntitySearchItem
*/
// Created: AGE 2006-08-10
// =============================================================================
class EntitySearchItem : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             EntitySearchItem( kernel::ActionController& actions, const kernel::Entity_ABC& entity );
    virtual ~EntitySearchItem();
    //@}

    //! @name Operations
    //@{
    bool Matches( const QString& input ) const;
    bool Matches( const kernel::Entity_ABC& entity ) const;
    void Activate();
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actions_;
    const kernel::Entity_ABC& entity_;
    QString name_;
    //@}
};

}

#endif // __EntitySearchItem_h_
