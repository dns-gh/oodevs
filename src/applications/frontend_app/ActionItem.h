// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionItem_h_
#define __ActionItem_h_

#pragma warning( push, 0)
#include <Qt3Support/q3action.h>
#include <QtGui/qtoolbutton.h>
#pragma warning( pop )

class QAction;

// =============================================================================
/** @class  ActionItem
    @brief  ActionItem
*/
// Created: SBO 2007-10-04
// =============================================================================
class ActionItem : public QToolButton
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionItem( QWidget* parent, Q3Action& action );
    virtual ~ActionItem();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionItem( const ActionItem& );            //!< Copy constructor
    ActionItem& operator=( const ActionItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Q3Action& action_;
    //@}
};

#endif // __ActionItem_h_
