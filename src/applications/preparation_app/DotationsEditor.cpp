// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DotationsEditor.h"
#include "moc_DotationsEditor.cpp"
#include "clients_kernel/DotationType.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "preparation/Dotation.h"
#include "preparation/DotationsItem.h"
#include <qtable.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DotationsEditor constructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::DotationsEditor( QDialog*& self, QWidget* parent, const tools::Resolver_ABC< DotationType, std::string >& dotationTypes, DotationsItem*& /*value*/ )
    : QDialog( parent, "DotationsEditor", true )
    , dotationTypes_( dotationTypes )
    , value_ ( 0 )
    , self_( self )
{
    setCaption( tr( "Dotations editor" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    QVBox* vBox = new QVBox( this );
    table_ = new QTable( 0, 2, vBox );
    table_->setColumnWidth( 0, 250 );
    table_->horizontalHeader()->setLabel( 0, tr( "Type" ) );
    table_->horizontalHeader()->setLabel( 1, tr( "Quantity" ) );
    table_->setMargin( 5 );

    QHBox* box = new QHBox( vBox );
    QButton* ok     = new QPushButton( tr( "Ok" ), box );
    QButton* cancel = new QPushButton( tr( "Cancel" ), box );
    pMainLayout->addWidget( vBox );

    tools::Iterator< const DotationType& > it = dotationTypes_.CreateIterator();
    types_.append( "" );
    while( it.HasMoreElements() )
        types_.append( it.NextElement().GetName().c_str() );

    connect( ok    , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    connect( table_, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );
    show();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor destructor
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsEditor::~DotationsEditor()
{
    self_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnAccept
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnAccept()
{
    (*value_)->Clear();
    for( int i = 0; i < table_->numRows() - 1; ++i )
        (*value_)->AddDotation( dotationTypes_.Get( table_->text( i, 0 ).ascii() ), table_->text( i, 1 ).toUInt() );
    (*value_)->Update();
    accept();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnReject
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::SetCurrentItem
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::SetCurrentItem( DotationsItem*& dotations )
{
    value_ = &dotations;
    table_->setNumRows( 0 );
    tools::Iterator< const Dotation& > it = dotations->CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        AddItem( &dotation );
    }
    AddItem();
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver< Dotation >* DotationsEditor::GetValue
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
DotationsItem* DotationsEditor::GetValue()
{
    return *value_;
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::sizeHint
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
QSize DotationsEditor::sizeHint() const
{
    return QSize( 400, 300 );
}

// -----------------------------------------------------------------------------
// Name: DotationsEditor::OnValueChanged
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::OnValueChanged( int row, int col )
{
    ExclusiveComboTableItem& item = *static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( col == 0 )
    {
        if( item.currentItem() == 0 && row != table_->numRows() - 1 )
        {
            table_->removeRow( row );
            table_->setCurrentCell( table_->numRows() - 1, 1 );
            return;
        }
        if( item.currentItem() && row == table_->numRows() - 1 )
        {
            const int current = item.currentItem();
            if( table_->numRows() < int( types_.size() ) - 1 )
                AddItem();
            item.setCurrentItem( current );
        }
        table_->setCurrentCell( row, 1 );
    }
}


// -----------------------------------------------------------------------------
// Name: DotationsEditor::AddItem
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void DotationsEditor::AddItem( const Dotation* dotation /*=0*/ )
{
    const unsigned int row = table_->numRows();
    table_->setNumRows( row + 1 );
    ExclusiveComboTableItem* item = new ExclusiveComboTableItem( table_, types_ );
    table_->setItem( row, 0, item );
    if( dotation )
    {
        table_->setText( row, 1, QString::number( dotation->quantity_ ) );
        item->setCurrentItem( dotation->type_->GetName().c_str() );
    }
    table_->setCurrentCell( row, 1 );
}
