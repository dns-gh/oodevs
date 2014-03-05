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

#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>
#include <boost/optional.hpp>

namespace gui
{
    class LogisticBase;
}

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  InfoDialog_Base
    @brief  InfoDialog_Base
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoDialog_Base : public QDialog
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< gui::LogisticBase >
{
    Q_OBJECT

protected:
    //! @name Types
    //@{
    typedef std::function< bool( const gui::LogisticBase& ) > T_LogisticBaseModeChecker;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             InfoDialog_Base( kernel::Controllers& controllers,
                              QWidget* parent,
                              const QString& title,
                              const T_LogisticBaseModeChecker& modeChecker = T_LogisticBaseModeChecker() );
    virtual ~InfoDialog_Base();
    //@}

    //! @name Operations
    //@{
    QWidget* RootWidget();
    //@}

protected:
    //! @name Helpers
    //@{
    void SetEnabled( bool state );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const gui::LogisticBase& ext );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const = 0;
    virtual QSize sizeHint() const;
    virtual void hideEvent( QHideEvent* );
    virtual void showEvent( QShowEvent* event );
    void UpdateTitle( boost::optional< bool > manual );
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
    void Shown();
    //@}

protected:
    //! @name Protected member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}

private:
    //! @name Member data
    //@{
    const QString baseTitle_;
    Q3VBox* box_;
    bool disconnected_;
    kernel::Controllers& controllers_;
    const T_LogisticBaseModeChecker modeChecker_;
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
{
public:
    //! @name Constructors/Destructor
    //@{
    InfoDialog( kernel::Controllers& controllers,
                QWidget* parent,
                const QString& title,
                const T_LogisticBaseModeChecker& modeChecker = T_LogisticBaseModeChecker() )
        : InfoDialog_Base( controllers, parent, title, modeChecker )
    {
        // NOTHING
    }
    virtual ~InfoDialog()
    {
        // NOTHING
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const
    {
        return element.Retrieve< Extension >() != 0;
    }
    //@}
};

#endif // __InfoDialog_h_
