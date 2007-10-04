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

#include <qtoolbutton.h>

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
             ActionItem( QWidget* parent, QAction& action );
    virtual ~ActionItem();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionItem( const ActionItem& );            //!< Copy constructor
    ActionItem& operator=( const ActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QAction& action_;
    //@}
};

#endif // __ActionItem_h_
