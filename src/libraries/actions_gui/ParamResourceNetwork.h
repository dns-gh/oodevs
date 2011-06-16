// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParamResourceNetwork_h_
#define __ParamResourceNetwork_h_

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class Controller;
    class Object_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamResourceNetwork
    @brief  ParamResourceNetwork
*/
// Created: JSR 2011-05-02
// =============================================================================
class ParamResourceNetwork : public QObject
                           , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                           , public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamResourceNetwork( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller );
    virtual ~ParamResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const;
    virtual void Show();
    virtual void Hide();
    //@}

public slots:
    //! @name Slots
    //@{
    void MenuItemValidated( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::OrderParameter parameter_;
    const kernel::Object_ABC* current_;
    const kernel::Object_ABC* selected_;
    QLabel* objectLabel_;
    QLabel* objectName_;
    QLabel* resourceLabel_;
    QLabel* resourceName_;
    //@}
};

    }
}

#endif // __ParamResourceNetwork_h_
