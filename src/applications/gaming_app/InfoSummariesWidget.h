// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSummariesWidget_h_
#define __InfoSummariesWidget_h_

#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
    class Displayer_ABC;
}

// =============================================================================
/** @class  InfoSummariesWidget
    @brief  InfoSummariesWidget
*/
// Created: SBO 2007-02-28
// =============================================================================
class InfoSummariesWidget : public QVBox
                          , public tools::Observer_ABC
                          , public tools::SelectionObserver< kernel::Entity_ABC >
                          , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                          , public kernel::ElementObserver_ABC< kernel::Attributes_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoSummariesWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InfoSummariesWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoSummariesWidget( const InfoSummariesWidget& );            //!< Copy constructor
    InfoSummariesWidget& operator=( const InfoSummariesWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Attributes_ABC& extension );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::auto_ptr< kernel::Displayer_ABC > display_;
    //@}
};

#endif // __InfoSummariesWidget_h_
