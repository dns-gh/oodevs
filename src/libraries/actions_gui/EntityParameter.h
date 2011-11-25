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
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Controller.h"

namespace kernel
{
    class OrderParameter;
}

namespace gui
{
    class RichLabel;
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
class EntityParameterBase : public QObject
                          , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EntityParameterBase( QObject* parent, const QString& name, bool optional );
    virtual ~EntityParameterBase();
    //@}

    //! @name Operations
    //@{
    virtual void Show();
    virtual void Hide();
    virtual QWidget* BuildInterface( QWidget* parent );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void AddToMenu( kernel::ContextMenu& menu );
    void Display( const QString& what );
    void SetLabel( const QString& label );
    //@}

private:
    //! @name Member data
    //@{
    ::gui::RichLabel* pLabel_;
    QLabel* entityLabel_; // $$$$ AGE 2006-03-14: LabelDisplayer ?
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
             EntityParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller );
             EntityParameter( QObject* parent, const kernel::OrderParameter& parameter, const ConcreteEntity& entity, kernel::Controller& controller );
    virtual ~EntityParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool InternalCheckValidity() const;
    void CommitTo( int& message ) const;
    void CommitTo( actions::parameters::Entity< ConcreteEntity >& parameter ) const;
    virtual void MenuItemValidated();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const ConcreteEntity& ) {};
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const ConcreteEntity* potential_;
    const ConcreteEntity* selected_;
    //@}
};

#include "EntityParameter.inl"

    }
}

#endif // __EntityParameter_h_
