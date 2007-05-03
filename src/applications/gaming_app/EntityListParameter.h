// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityListParameter_h_
#define __EntityListParameter_h_

#include "game_asn/Asn.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/ActionParameter.h"
#include "gaming/Action_ABC.h"

// =============================================================================
/** @class  EntityListParameterBase
    @brief  EntityListParameterBase
*/
// Created: AGE 2006-03-14
// =============================================================================
class EntityListParameterBase : public QObject
                              , public Param_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameterBase( QObject* parent, const QString& name );
    virtual ~EntityListParameterBase();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void BuildInterface( QWidget* parent );
    //@}

private slots:
    //! @name slots
    //@{
    virtual void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    void OnDeleteSelectedItem();
    void OnClearList();
    void TurnHeaderBlack();
    virtual void MenuItemValidated() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void AddToMenu( kernel::ContextMenu& menu );
    virtual bool Invalid();
    virtual unsigned long GetId( gui::ValuedListItem* item ) const = 0;
    virtual void CommitTo( ASN1T_ListOID*& asn ) const;
    virtual void Clean( ASN1T_ListOID*& asn ) const;
    //@}

protected:
    //! @name Member data
    //@{
    QListView* listView_;
    //@}

private:
    //! @name Member data
    //@{
    QPopupMenu* pPopupMenu_;
    //@}
};

// =============================================================================
/** @class  EntityListParameter
    @brief  EntityListParameter
*/
// Created: AGE 2006-03-14
// =============================================================================
template< typename ConcreteEntity >
class EntityListParameter : public EntityListParameterBase
                          , public kernel::ContextMenuObserver_ABC< ConcreteEntity >
                          , public kernel::ElementObserver_ABC< ConcreteEntity >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityListParameter( QObject* pParent, const kernel::OrderParameter& parameter );
    virtual ~EntityListParameter();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Action_ABC& action ) const;
    using EntityListParameterBase::CommitTo;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityListParameter( const EntityListParameter& );            //!< Copy constructor
    EntityListParameter& operator=( const EntityListParameter& ); //!< Assignement operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu );
    virtual void MenuItemValidated();
    virtual void NotifyUpdated( const ConcreteEntity& ) {};
    virtual void NotifyDeleted( const ConcreteEntity& entity );
    virtual unsigned long GetId( gui::ValuedListItem* item ) const;
    //@}

private:
    //! @name Member data
    //@{
    const ConcreteEntity* potential_;
    const kernel::OrderParameter& parameter_;
    //@}
};

#include "EntityListParameter.inl"

#endif // __EntityListParameter_h_
