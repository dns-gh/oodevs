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

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "tools/SelectionObserver_ABC.h"

// =============================================================================
/** @class  InfoDialog_Base
    @brief  InfoDialog_Base
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoDialog_Base : public QDialog
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
    Q3VBox* box_;
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
                 , public tools::Observer_ABC
                 , public tools::SelectionObserver< kernel::Entity_ABC >
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
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::ShouldDisplay
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename Extension >
bool InfoDialog< Extension >::ShouldDisplay( const kernel::Entity_ABC& element ) const
{
    return element.Retrieve< Extension >() != 0;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename Extension >
void InfoDialog< Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    SetEnabled( element && ShouldDisplay( *element ) );
}

#endif // __InfoDialog_h_
