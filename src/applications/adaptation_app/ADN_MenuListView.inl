// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_MenuListView constructor
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
template< typename T >
ADN_MenuListView< T >::ADN_MenuListView( ADN_Table3* receiver, ADN_Type_Vector_ABC< T >& vector, QObject* parent )
    : QWidgetAction( parent )
    , vector_( vector )
    , receiver_( receiver )
    , listView_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MenuListView destructor
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
template< typename T >
ADN_MenuListView< T >::~ADN_MenuListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MenuListView::createWidget
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
template< typename T >
inline
QWidget* ADN_MenuListView< T >::createWidget( QWidget* parent )
{
    listView_ = new QListWidget( parent );
    listView_->setMinimumSize( 300, std::min( 600, static_cast< int >( vector_.size() ) * 20 ) );

    for( ADN_Type_Vector_ABC< T >::iterator it = vector_.begin(); it != vector_.end(); ++it )
    {
        QListWidgetItem* item = new QListWidgetItem( (*it)->strName_.GetData().c_str() );
        item->setData( gui::Roles::OtherRole, QVariant::fromValue( *it ) );
        listView_->addItem( item );
        for( int i = 0; i < receiver_->numRows(); ++i )
        {
            QStandardItem* receiverItem = receiver_->GetItem( i, 0 );
            if( receiverItem && receiverItem->text().toStdString() == (*it)->strName_.GetData() )
            {
                item->setFlags( Qt::NoItemFlags );
                break;
            }
        }
    }
    connect( listView_, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), receiver_, SLOT( MenuListItemSelected() ) );
    connect( listView_, SIGNAL( itemEntered( QListWidgetItem* ) ), receiver_, SLOT( MenuListItemSelected() ) );
    return listView_;
}

// -----------------------------------------------------------------------------
// Name: ADN_MenuListView::SelectedValue
// Created: ABR 2012-11-06
// -----------------------------------------------------------------------------
template< typename T >
inline
T* ADN_MenuListView< T >::SelectedValue() const
{
    if( listView_ )
        if( QListWidgetItem* item = listView_->currentItem() )
            return item->data( gui::Roles::OtherRole ).value< T* >();
    return 0;
}
