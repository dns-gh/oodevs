// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui_PropertyDisplayer_h_
#define gui_PropertyDisplayer_h_

#include "NoLinkDisplayer.h"

namespace gui
{
// =============================================================================
/** @class  PropertyDisplayer
    @brief  Property displayer
*/
// Created: SBO 2006-10-18
// =============================================================================
class PropertyDisplayer : public NoLinkDisplayer
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertyDisplayer();
    virtual ~PropertyDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide() ;
    void SetItem( QStandardItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual kernel::Displayer_ABC& SubItem( const QString& name ) ;
    virtual void StartDisplay() ;
    virtual void DisplayFormatted( const QString& formatted ) ;
    virtual void EndDisplay() ;
    //@}

protected:
    //! @name Member data
    //@{
    QString        message_;
    QStandardItem*   item_;
    QColor         color_;
    //@}
};

}

#endif // gui_PropertyDisplayer_h_
