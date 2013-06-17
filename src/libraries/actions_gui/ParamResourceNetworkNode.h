// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ParamResourceNetworkNode_h_
#define __ParamResourceNetworkNode_h_

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class Controller;
    class Object_ABC;
    class UrbanObject_ABC;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamResourceNetworkNode
    @brief  ParamResourceNetworkNode
*/
// Created: JSR 2011-05-02
// =============================================================================
class ParamResourceNetworkNode : public Param_ABC
                               , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                               , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamResourceNetworkNode( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamResourceNetworkNode();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const;
    virtual bool InternalCheckValidity() const;
    virtual void CreateInternalMenu( kernel::ContextMenu& menu );
    virtual void Visit( const actions::parameters::Resource& param );
    virtual void Visit( const actions::parameters::ResourceNetworkNode& param );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC& entity, kernel::ContextMenu& menu );
    void DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
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
    const kernel::Entity_ABC* current_;
    const kernel::Entity_ABC* selected_;
    QLabel* objectName_;
    QLabel* resourceName_;
    T_Actions actions_;
    //@}
};

}
}

#endif // __ParamResourceNetworkNode_h_
