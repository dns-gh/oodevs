// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WidgetStack_h_
#define __WidgetStack_h_

#include <qwidgetstack.h>

// =============================================================================
/** @class  WidgetStack
    @brief  WidgetStack
*/
// Created: SBO 2007-10-04
// =============================================================================
class WidgetStack : public QWidgetStack
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit WidgetStack( QWidget* parent );
    virtual ~WidgetStack();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WidgetStack( const WidgetStack& );            //!< Copy constructor
    WidgetStack& operator=( const WidgetStack& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __WidgetStack_h_
