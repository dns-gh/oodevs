// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "clients_gui_pch.h"
#include "LoadableField.h"
#include "moc_LoadableField.cpp"
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableField constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableField::LoadableField( QWidget* parent, const QString& probableName)
: QHBox( parent )
, probableName_( probableName )
, defaultValueWidget_( 0 )
, currentLoader_( 0 )
{
    fieldName_ = new QComboBox( this );
    connect( fieldName_, SIGNAL( activated(const QString&) ), this, SLOT( FieldSelected(const QString&) ) );
    SetPossibleFields(  QStringList() );
}

// -----------------------------------------------------------------------------
// Name: LoadableField destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableField::~LoadableField()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableField::SetLoader
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
void LoadableField::SetLoader( const ObjectPrototypeLoader_ABC* loader )
{
    currentLoader_ = loader;
    SetPossibleFields( loader ? loader->GetFields() : QStringList() );
    SelectProbableField();
}

// -----------------------------------------------------------------------------
// Name: LoadableField::SetPossibleFields
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void LoadableField::SetPossibleFields( const QStringList& fields )
{
    fieldName_->clear();
    if( fields.empty() )
        fieldName_->hide();
    else
    {
        fieldName_->show();
        fieldName_->insertItem( QString::null );
        fieldName_->insertStringList( fields );
    }
}

// -----------------------------------------------------------------------------
// Name: LoadableField::GetField
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
QString LoadableField::GetField() const
{
    return fieldName_->currentText();
}

// -----------------------------------------------------------------------------
// Name: LoadableField::FieldSelected
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
void LoadableField::FieldSelected( const QString& fieldName )
{
    if( defaultValueWidget_ )
        defaultValueWidget_->setEnabled( fieldName.isNull() );
}

// -----------------------------------------------------------------------------
// Name: LoadableField::SelectProbableField
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
void LoadableField::SelectProbableField()
{
    if( !probableName_.isNull() )
    {
        for( int i=0, count=fieldName_->count(); i<count; ++i )
        {
            QString text = fieldName_->text( i );
            if( text.contains( probableName_, false ) )
            {
                fieldName_->setCurrentItem( i );
                FieldSelected( text );
                return;
            }
        }
    }
    FieldSelected( QString::null );
}
