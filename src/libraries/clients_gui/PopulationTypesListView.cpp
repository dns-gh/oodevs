// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationTypesListView.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PopulationPrototype.h"
#include "clients_kernel/PopulationType.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationTypesListView::PopulationTypesListView( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, const QSpinBox& number )
    : RichTreeView( parent, &controllers )
    , controllers_( controllers )
    , types_( types )
    , number_( number )
    , prototype_( new kernel::PopulationPrototype )
{
    EnableDragAndDrop( true );
    setHeaderHidden( true );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationTypesListView::~PopulationTypesListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const kernel::PopulationType& > it( types_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const kernel::PopulationType & type = it.NextElement();
        dataModel_.AddRootDataItem( dataModel_.rowCount(), 0, type.GetName().c_str(), "", type, Qt::ItemIsDragEnabled );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    dataModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::MimeTypes
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
QStringList PopulationTypesListView::MimeTypes() const
{
    return QStringList( typeid( kernel::PopulationType ).name() );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::MimeData
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
QMimeData* PopulationTypesListView::MimeData( const QModelIndexList& indexes, bool& overriden ) const
{
    overriden = true;
    if( indexes.size() )
    {
        prototype_->type_ = dataModel_.GetDataFromIndex< kernel::PopulationType >( indexes.at( 0 ) );
        prototype_->number_ = number_.value();
        QMimeData* mimeData = new QMimeData();
        QByteArray encodedData;
        QDataStream stream( &encodedData, QIODevice::WriteOnly );
        stream << reinterpret_cast< unsigned int >( prototype_.get() );
        mimeData->setData( QString( typeid( kernel::PopulationPrototype ).name() ), encodedData );
        return mimeData;
    }
    return 0;
}
