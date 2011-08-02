// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoEventsWidget_h_
#define __InfoEventsWidget_h_

#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
}

class Attributes;
class Contaminations;

// =============================================================================
/** @class  InfoEventsWidget
    @brief  InfoEventsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoEventsWidget : public Q3VBox
                       , public tools::Observer_ABC
                       , public tools::SelectionObserver< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< kernel::Attributes_ABC >
                       , public tools::ElementObserver_ABC< Contaminations >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoEventsWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InfoEventsWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoEventsWidget( const InfoEventsWidget& );            //!< Copy constructor
    InfoEventsWidget& operator=( const InfoEventsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Attributes_ABC& element );
    virtual void NotifyUpdated( const Contaminations& element );

    void InitializeEvents( QWidget* parent );
    QLabel* CreateEvent( QWidget* parent, const QPixmap& pixmap, const QString& text );
    void ToggleEvent( const std::string& event, bool toggle );

    void SetAttributes( const Attributes& attributes );
    void SetContaminations( const Contaminations& attributes );
    void SetShown();
    void Reset();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, QLabel* > T_Events;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    T_Events events_;
    //@}
};

#endif // __InfoEventsWidget_h_
