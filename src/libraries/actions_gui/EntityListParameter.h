// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityListParameter_h_
#define __EntityListParameter_h_

#include "ListParameter.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class ContextMenu;
    class OrderParameter;
    class ActionController;
    class Controller;
}

namespace actions
{
    namespace gui
    {
    template< typename T > class EntityParameter;

// =============================================================================
/** @class  EntityListParameterBase
    @brief  EntityListParameterBase
*/
// Created: SBO 2007-05-23
// =============================================================================
class EntityListParameterBase : public ListParameter
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameterBase( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller );
    virtual ~EntityListParameterBase();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu ) = 0;
    void MakeMenu( const QString& label, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated() = 0;
    //@}
};

// =============================================================================
/** @class  EntityListParameter
    @brief  EntityListParameter
*/
// Created: SBO 2007-05-23
// =============================================================================
template< typename ConcreteEntity >
class EntityListParameter : public EntityListParameterBase
                          , public tools::ElementObserver_ABC< ConcreteEntity >
                          , public kernel::ContextMenuObserver_ABC< ConcreteEntity >
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller );
    virtual ~EntityListParameter();
    //@}

protected:
    //! @name Helpers
    //@{
    void Add( const ConcreteEntity& entity );
    virtual void NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    virtual EntityParameter< ConcreteEntity >* CreateElement( const ConcreteEntity& potential ) = 0;
    virtual void MenuItemValidated();
    virtual void DeleteElement( Param_ABC& param );
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    //@}

    //! @name Types
    //@{
    typedef std::map< const ConcreteEntity*, EntityParameter< ConcreteEntity >* > T_Entities;
    typedef typename T_Entities::const_iterator                                 CIT_Entities;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}

private:
    //! @name Member data
    //@{
    const ConcreteEntity* potential_; // $$$$ SBO 2007-05-23: safe pointer
    T_Entities entities_;
    //@}
};

#include "EntityListParameter.inl"

    }
}

#endif // __EntityListParameter_h_
