// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeView_h_
#define __AttributeView_h_

#include "clients_gui/NoLinkDisplayer.h"
#include "gaming/Simulation.h"
#include "clients_kernel/ElementObserver_ABC.h"

#include <qtable.h>

namespace kernel
{
    class Entity_ABC;
    class DataDictionary;
    class Controllers;
}

// =============================================================================
/** @class  AttributeView
    @brief  AttributeView
*/
// Created: AGE 2006-06-22
// =============================================================================
class AttributeView : public QTable
                    , public gui::NoLinkDisplayer
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AttributeView( QWidget* parent, kernel::Controllers& controllers, const kernel::Entity_ABC& agent );
    virtual ~AttributeView();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AttributeView( const AttributeView& );            //!< Copy constructor
    AttributeView& operator=( const AttributeView& ); //!< Assignement operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int, int );
    //@}

private:
    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    virtual void NotifyUpdated( const Simulation::sEndTick& );
    virtual void keyPressEvent( QKeyEvent* );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DataDictionary& dictionary_;
    int currentRow_;
    QString message_;
    //@}
};

#endif // __AttributeView_h_
