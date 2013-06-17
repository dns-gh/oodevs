// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityParameter_h_
#define __EntityParameter_h_

#include "Param_ABC.h"
#include "actions/Entity.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Controller.h"
#include "InterfaceBuilder_ABC.h"
#include "clients_kernel/Controllers.h"

namespace kernel
{
    class OrderParameter;
}

namespace actions
{
    namespace parameters
    {
        template< typename T > class Entity;
    }
    namespace gui
    {

// =============================================================================
/** @class  EntityParameterBase
    @brief  EntityParameterBase
*/
// Created: AGE 2006-03-14
// =============================================================================
class EntityParameterBase : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityParameterBase( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~EntityParameterBase();
    //@}

    //! @name Operations
    //@{
    virtual void Show();
    virtual void Hide();
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    //@}

protected:
    //! @name Helpers
    //@{
    void Display( const QString& what );
    //@}

protected:
    //! @name Member data
    //@{
    QLabel* entityLabel_;
    //@}
};

// =============================================================================
/** @class  EntityParameter
    @brief  EntityParameter
*/
// Created: AGE 2006-03-14
// =============================================================================
template< typename ConcreteEntity >
class EntityParameter : public EntityParameterBase
                      , public kernel::ContextMenuObserver_ABC< ConcreteEntity >
                      , public tools::ElementObserver_ABC< ConcreteEntity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~EntityParameter();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void SetPotential( const ConcreteEntity& entity );
    void CommitTo( int& message ) const;
    void CommitTo( actions::parameters::Entity< ConcreteEntity >& parameter ) const;
    virtual void OnMenuClick();
    //@}

    //! @name ContextMenuObserver< ConcreteEntity > implementation
    //@{
    virtual void NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu );
    //@}

protected:
    //! @name ElementObserver_ABC< ConcreteEntity > implementation
    //@{
    virtual void NotifyUpdated( const ConcreteEntity& ) {};
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    //@}

    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    virtual void InternalVisit( const actions::parameters::Entity< ConcreteEntity >& param );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}

private:
    //! @name Member data
    //@{
    const ConcreteEntity* potential_;
    const ConcreteEntity* selected_;
    //@}
};

#include "EntityParameter.inl"

    }
}

#endif // __EntityParameter_h_
