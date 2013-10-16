// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSubordinateItem_h_
#define __InfoSubordinateItem_h_

#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
}

namespace gui
{
    class EntitySymbols;
}

class Attributes;

// =============================================================================
/** @class  InfoSubordinateItem
    @brief  InfoSubordinateItem
*/
// Created: SBO 2007-02-22
// =============================================================================
class InfoSubordinateItem : public QListWidgetItem
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< Attributes >
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoSubordinateItem( QListWidget* parent, kernel::Controllers& controllers, gui::EntitySymbols& icons, const kernel::Entity_ABC& entity );
    virtual ~InfoSubordinateItem();
    //@}

    //! @name Operations
    //@{
    const kernel::Entity_ABC& GetEntity() const;
    virtual QVariant data( int role ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Attributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::EntitySymbols& icons_;
    int oldRawState_;
    //@}
};

#endif // __InfoSubordinateItem_h_
