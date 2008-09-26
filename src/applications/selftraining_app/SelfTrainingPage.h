// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelfTrainingPage_h_
#define __SelfTrainingPage_h_

#include "MenuPage.h"

namespace tools
{
    class GeneralConfig;
}

namespace gui 
{
    class LinkInterpreter_ABC ; 
}

class Session; 

// =============================================================================
/** @class  SelfTrainingPage
    @brief  SelfTrainingPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class SelfTrainingPage : public MenuPage
{

public:
    //! @name Constructors/Destructor
    //@{
             SelfTrainingPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< Session > sessionStatus );
    virtual ~SelfTrainingPage();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SelfTrainingPage( const SelfTrainingPage& );            //!< Copy constructor
    SelfTrainingPage& operator=( const SelfTrainingPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __SelfTrainingPage_h_
