// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectTreeView.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectTreeView constructor
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
ObjectTreeView::ObjectTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( objectName, controllers, profile, modelObserver, parent )
{
    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView destructor
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
ObjectTreeView::~ObjectTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::NotifyCreated
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void ObjectTreeView::NotifyCreated( const kernel::Object_ABC& object )
{
    const kernel::TacticalHierarchies* hierarchies = object.Retrieve< kernel::TacticalHierarchies >();
    if( !hierarchies ) // urban block or other object not attached to a side
        return;
    const kernel::Entity_ABC& team = hierarchies->GetTop();
    QStandardItem* teamItem = dataModel_.FindDataItem( team );
    if( !teamItem )
        teamItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, team.GetName(), team.GetTooltip(), team );
    const kernel::ObjectType& type = object.GetType();
    QStandardItem* typeItem = dataModel_.FindTextItem( type.GetName().c_str(), teamItem );
    if( !typeItem )
        typeItem = dataModel_.AddChildTextItem( teamItem, teamItem->rowCount(), 0, type.GetName().c_str(), type.GetName().c_str() );
    dataModel_.AddChildSafeItem( typeItem, typeItem->rowCount(), 0, object.GetName(), object.GetTooltip(), object, ItemSpecificFlags( object ) );
    ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::NotifyDeleted
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void ObjectTreeView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    EntityTreeView_ABC::NotifyDeleted( entity );
    if( !IsTypeRejected( entity ) )
    {
        const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchies ) // urban block or other object not attached to a side
            return;
        QStandardItem* teamItem = dataModel_.FindDataItem( hierarchies->GetTop() );
        if( !teamItem )
            return;
        for( int row = 0; row < teamItem->rowCount(); )
        {
            QStandardItem* childItem = teamItem->child( row, 0 );
            if( childItem )
            {
                if( childItem->rowCount() == 0 )
                {
                    QList< QStandardItem* > rowItems = teamItem->takeRow( row );
                    for( QList< QStandardItem *>::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
                        delete *it;
                }
                else
                    ++row;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::MimeTypes
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
QStringList ObjectTreeView::MimeTypes() const
{
    return QStringList();
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::IsTypeRejected
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
bool ObjectTreeView::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != kernel::Object_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: ObjectTreeView::LessThan
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
bool ObjectTreeView::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    valid = true;
    const kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    const kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( entity1 && entity2 )
        return entity2->GetId() < entity1->GetId();
    QString txt1 = dataModel_.data( left ).toString();
    QString txt2 = dataModel_.data( right ).toString();
    return txt1.localeAwareCompare( txt2 ) > 0;
}
