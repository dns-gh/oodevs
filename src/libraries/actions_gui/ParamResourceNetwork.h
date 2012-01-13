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
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamResourceNetwork
    @brief  ParamResourceNetwork
*/
// Created: JSR 2011-05-02
// =============================================================================
class ParamResourceNetwork : public Param_ABC
                           , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamResourceNetwork( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamResourceNetwork();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const;
    virtual bool InternalCheckValidity() const;
    virtual void CreateInternalMenu( kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    void UpdateLabel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMenuClick( QAction* action );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Object_ABC* current_;
    const kernel::Object_ABC* selected_;
    QLabel* objectName_;
    QLabel* resourceName_;
    T_Actions actions_;
    //@}
};

}
}

#endif // __ParamResourceNetwork_h_
