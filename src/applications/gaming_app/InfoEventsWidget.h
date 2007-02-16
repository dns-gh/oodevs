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

#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
}

class Attributes;
class Contaminations;
class InfoStancesWidget;

// =============================================================================
/** @class  InfoEventsWidget
    @brief  InfoEventsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoEventsWidget : public QVBox
                       , public kernel::Observer_ABC
                       , public kernel::SelectionObserver< kernel::Entity_ABC >
                       , public kernel::ElementObserver_ABC< kernel::Attributes_ABC >
                       , public kernel::ElementObserver_ABC< Contaminations >
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

    void AddEventButton( const std::string& event, bool add = true );
    QButton* MakeButton( QWidget* parent, const QPixmap& pixmap, const QString& text );

    void InitializeEventButtons( QWidget* parent );
    void SetAttributes( const Attributes& attributes );
    void SetContaminations( const Contaminations& attributes );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, QButton* > T_EventButtons;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    QLabel* speed_;
    QLabel* height_;

    T_EventButtons eventButtons_;
    InfoStancesWidget* stances_;
    //@}
};

#endif // __InfoEventsWidget_h_
