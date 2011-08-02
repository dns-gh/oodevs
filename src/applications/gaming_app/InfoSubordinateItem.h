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

#include "tools/ElementObserver_ABC.h"
#include <Qt3Support/q3iconview.h>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Attributes_ABC;
}

namespace gui
{
    class EntitySymbols;
}

// =============================================================================
/** @class  InfoSubordinateItem
    @brief  InfoSubordinateItem
*/
// Created: SBO 2007-02-22
// =============================================================================
class InfoSubordinateItem : public Q3IconViewItem
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::Attributes_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoSubordinateItem( Q3IconView* parent, kernel::Controllers& controllers, gui::EntitySymbols& icons, const kernel::Entity_ABC& entity );
    virtual ~InfoSubordinateItem();
    //@}

    //! @name Operations
    //@{
    const kernel::Entity_ABC& GetEntity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoSubordinateItem( const InfoSubordinateItem& );            //!< Copy constructor
    InfoSubordinateItem& operator=( const InfoSubordinateItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Attributes_ABC& attributes );
    virtual void paintItem( QPainter* p, const QColorGroup& cg );
    void DrawLife( QPainter* p, int life );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::EntitySymbols& icons_;
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __InfoSubordinateItem_h_
