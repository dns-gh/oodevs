// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __EntityTreeView_ABC_h_
#define __EntityTreeView_ABC_h_

#include "RichTreeView.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
    class Team_ABC;
}

namespace gui
{
class ModelObserver_ABC;

// =============================================================================
/** @class  EntityTreeView_ABC
    @brief  EntityTreeView_ABC
*/
// Created: ABR 2012-08-14
// =============================================================================
class EntityTreeView_ABC : public RichTreeView
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                         , public tools::ElementObserver_ABC< kernel::Team_ABC >
                         , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                         , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                         , public kernel::MultipleSelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    typedef boost::function< bool( const kernel::Entity_ABC&, const kernel::Entity_ABC& ) > T_LessThanEntityFunctor;
    //@}

    //! @name Constructors/Destructor
    //@{
             EntityTreeView_ABC( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                 ModelObserver_ABC& modelObserver, QWidget* parent = 0 );
    virtual ~EntityTreeView_ABC();
    //@}

    //! @name ActivationObserver_ABC< kernel::Entity_ABC >
    //@{
    virtual void NotifyActivated( const kernel::Entity_ABC& entity );
    //@}

    //! @name Element_Observer_ABC< kernel::Entity_ABC >
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Entity_ABC& team );
    //@}

    //! @name Element_Observer_ABC< kernel::Entity_ABC >
    //@{
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    //@}

    //! @name Element_Observer_ABC< kernel::Profile_ABC >
    //@{
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    //@}

    //! @name MultipleSelectionObserver
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements );
    //@}

protected:
    //! @name QWidget
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

    //! @name Operations
    //@{
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    virtual void ContextMenuRequested( const QPoint& /*where*/ ) {}
    virtual void ApplyProfileFilter();
    virtual bool ApplyProfileFilter( QStandardItem& item ) const;
    virtual void SetLessThanEntityFunctor( const T_LessThanEntityFunctor& functor );
    virtual void Edit( const kernel::Entity_ABC& entity );
    virtual void Rename( kernel::Entity_ABC& entity, const QString& name );
    virtual void Rename( kernel::Entity_ABC& entity );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnActivate( const QModelIndex& index );
    virtual void OnSelect( const QItemSelection& selected, const QItemSelection& deselected );
    void OnDataChanged( const QModelIndex& index, const QVariant& value );
    void ForceRedraw();
    void OnScrollToSelected();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    ModelObserver_ABC& modelObserver_;
    bool blockSelect_;
    //@}
};

} //! namespace gui

#endif // __EntityTreeView_ABC_h_
