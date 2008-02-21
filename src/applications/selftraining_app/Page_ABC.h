// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Page_ABC_h_
#define __Page_ABC_h_

#include <qvbox.h>

class QWidgetStack;

// =============================================================================
/** @class  Page_ABC
    @brief  Page_ABC
*/
// Created: SBO 2008-02-21
// =============================================================================
class Page_ABC : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Page_ABC( QWidgetStack* pages );
    virtual ~Page_ABC();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Page_ABC( const Page_ABC& );            //!< Copy constructor
    Page_ABC& operator=( const Page_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void Update() {};
    //@}

private:
    //! @name Member data
    //@{
    QWidgetStack* pages_;
    //@}
};

#endif // __Page_ABC_h_
