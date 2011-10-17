// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FuneralConsignsWidget_h_
#define __FuneralConsignsWidget_h_

#include "gaming/LogFuneralConsign.h"
#include "LogisticConsignsWidget_ABC.h"

// =============================================================================
/** @class  FuneralConsignsWidget
    @brief  FuneralConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class FuneralConsignsWidget : public LogisticConsignsWidget_ABC< FuneralConsignsWidget, LogFuneralConsign, LogFuneralConsigns >
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~FuneralConsignsWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FuneralConsignsWidget( const FuneralConsignsWidget& );            //!< Copy constructor
    FuneralConsignsWidget& operator=( const FuneralConsignsWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    //@}
};

#endif // __FuneralConsignsWidget_h_
