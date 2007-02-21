// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoDialog_h_
#define __InfoDialog_h_

#include "clients_gui/PanelStack_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"

// =============================================================================
/** @class  InfoDialog_Base
    @brief  InfoDialog_Base
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoDialog_Base : public QDialog
                      , public gui::PanelStack_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoDialog_Base( QWidget* parent, const QString& title );
    virtual ~InfoDialog_Base();
    //@}

    //! @name Operations
    //@{
    virtual void Add( QWidget* widget );
    virtual void Remove( QWidget* widget );
    virtual QSize sizeHint() const;
    virtual void hideEvent( QHideEvent* );
    QWidget* RootWidget(); // $$$$ SBO 2007-02-19: 
    //@}

public slots:
    //! @name Slots
    //@{
    void OnToggle( bool state );
    //@}

signals:
    //! @name Signal
    //@{
    void Closed();
    void Disabled( bool );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoDialog_Base( const InfoDialog_Base& );            //!< Copy constructor
    InfoDialog_Base& operator=( const InfoDialog_Base& ); //!< Assignment operator
    //@}

protected:
    //! @name Operations
    //@{
    void SetEnabled( bool state );
    //@}

private:
    //! @name Member data
    //@{
    QVBox* box_;
    bool disconnected_;
    //@}
};


// =============================================================================
/** @class  InfoDialog_Base
    @brief  InfoDialog_Base
*/
// Created: SBO 2007-02-15
// =============================================================================
template< typename Extension >
class InfoDialog : public InfoDialog_Base
                 , public kernel::Observer_ABC
                 , public kernel::SelectionObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoDialog( QWidget* parent, kernel::Controllers& controllers, const QString& title );
    virtual ~InfoDialog();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoDialog( const InfoDialog& );            //!< Copy constructor
    InfoDialog& operator=( const InfoDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: InfoDialog constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename Extension >
InfoDialog< Extension >::InfoDialog( QWidget* parent, kernel::Controllers& controllers, const QString& title )
    : InfoDialog_Base( parent, title )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename Extension >
InfoDialog< Extension >::~InfoDialog()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename Extension >
void InfoDialog< Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    SetEnabled( element && element->Retrieve< Extension >() );
}

#endif // __InfoDialog_h_
