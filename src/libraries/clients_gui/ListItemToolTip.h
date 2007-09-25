// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemToolTip_h_
#define __ListItemToolTip_h_

#include <qtooltip.h>

// =============================================================================
/** @class  ListItemToolTip
    @brief  ListItemToolTip
*/
// Created: SBO 2007-01-08
// =============================================================================
class ListItemToolTip : public QToolTip
{

public:
    //! @name Constructors/Destructor
    //@{
             ListItemToolTip( QWidget* parent, QListView& list );
    virtual ~ListItemToolTip();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ListItemToolTip( const ListItemToolTip& );            //!< Copy constructor
    ListItemToolTip& operator=( const ListItemToolTip& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void maybeTip( const QPoint& pos );
    //@}

private:
    //! @name Member data
    //@{
    QListView& listView_;
    //@}
};

#endif // __ListItemToolTip_h_
