// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoStatusWidget_h_
#define __InfoStatusWidget_h_

#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Attributes_ABC;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;
}

class QProgressBar;
class Attributes;

// =============================================================================
/** @class  InfoStatusWidget
    @brief  InfoStatusWidget
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoStatusWidget : public QVBox
                       , public kernel::Observer_ABC
                       , public kernel::SelectionObserver< kernel::Entity_ABC >
                       , public kernel::ElementObserver_ABC< kernel::Attributes_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoStatusWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
    virtual ~InfoStatusWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void GotoParent();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoStatusWidget( const InfoStatusWidget& );            //!< Copy constructor
    InfoStatusWidget& operator=( const InfoStatusWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Attributes_ABC& element );

    void SetDefault();
    void SetIcon   ();
    void SetLifeBar( const Attributes& attributes );
    void SetName   ( const Attributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    gui::EntitySymbols& icons_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;

    QPushButton* gotoParent_;
    QLabel* icon_;
    QLabel* name_;
    QProgressBar* lifeBar_;

    QPixmap csword_;
    //@}
};

#endif // __InfoStatusWidget_h_
