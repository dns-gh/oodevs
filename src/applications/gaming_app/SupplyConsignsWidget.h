// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyConsignsWidget_h_
#define __SupplyConsignsWidget_h_

#include "gaming/LogSupplyConsign.h"
#include "LogisticConsignsWidget_ABC.h"

class SupplyStates;
class DotationRequest;

// =============================================================================
/** @class  SupplyConsignsWidget
    @brief  SupplyConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class SupplyConsignsWidget : public LogisticConsignsWidget_ABC< SupplyConsignsWidget, LogSupplyConsign, SupplyStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyConsignsWidget();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const LogSupplyConsign* consign, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const DotationRequest& request, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyConsignsWidget( const SupplyConsignsWidget& );            //!< Copy constructor
    SupplyConsignsWidget& operator=( const SupplyConsignsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< SupplyConsignsWidget >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< SupplyConsignsWidget >* list );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    //@}
};

#endif // __SupplyConsignsWidget_h_
