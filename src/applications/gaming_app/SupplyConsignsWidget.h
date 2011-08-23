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

class SupplyRecipientResourcesRequest;
class SupplyResourceRequest;

// =============================================================================
/** @class  SupplyConsignsWidget
    @brief  SupplyConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class SupplyConsignsWidget : public LogisticConsignsWidget_ABC< SupplyConsignsWidget, LogSupplyConsign, LogSupplyConsigns >
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
    void Display( const SupplyRecipientResourcesRequest& request, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const SupplyResourceRequest& request, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyConsignsWidget( const SupplyConsignsWidget& );            //!< Copy constructor
    SupplyConsignsWidget& operator=( const SupplyConsignsWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    //@}
};

#endif // __SupplyConsignsWidget_h_
