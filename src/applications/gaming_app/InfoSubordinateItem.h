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

#include <qiconview.h>

namespace kernel
{
    class Entity_ABC;
}

class Lives;

// =============================================================================
/** @class  InfoSubordinateItem
    @brief  InfoSubordinateItem
*/
// Created: SBO 2007-02-22
// =============================================================================
class InfoSubordinateItem : public QIconViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoSubordinateItem( QIconView* parent, const QString& text, const QPixmap& icon, const kernel::Entity_ABC& entity );
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
    virtual void paintItem( QPainter* p, const QColorGroup& cg );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __InfoSubordinateItem_h_
