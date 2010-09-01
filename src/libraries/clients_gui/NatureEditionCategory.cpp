// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NatureEditionCategory.h"
#include "moc_NatureEditionCategory.cpp"
#include "Tools.h"
#include "clients_kernel/SymbolRule.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory constructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionCategory::NatureEditionCategory( QWidget* parent, const kernel::SymbolRule* rule )
    : QHBox( parent )
    , rule_( rule )
    , current_( QString::null )
    , next_( 0 )
{
    label_ = new QLabel( this );
    label_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Preferred );
    box_ = new QComboBox( this );
    box_->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Preferred );
    setStretchFactor( label_, 1 );
    setStretchFactor( box_, 1 );
    setStretchFactor( new QWidget( this ), 1 );
    rule_->Accept( *this );

    connect( box_, SIGNAL( activated( int ) ), this, SLOT( OnComboChange() ) );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionCategory::~NatureEditionCategory()
{
    delete next_;
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::OnComboChange
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::OnComboChange()
{
    const QString current = box_->currentText();
    if( tools::translate( "models::app6", current_ ) != current )
    {
        current_ = internalNames_[current].c_str();
        delete next_; next_ = 0;
        rule_->Accept( *this );
        emit NatureChanged( current_ );
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::StartCategory
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::StartCategory( const std::string& title )
{
    if( current_.isNull() )
    {
        label_->setText( tools::translate( "models::app6", title.c_str() ) );
        box_->clear();
        box_->insertItem( tools::translate( "models::app6", "undefined" ) );
        internalNames_.clear();
        internalNames_[ tools::translate( "models::app6", "undefined" ) ] = "undefined";
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::AddChoice
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::AddChoice( SymbolRule* rule, const std::string& name, const std::string& )
{
    if( current_.isNull() )
    {
        QString translatedName = tools::translate( "models::app6", name.c_str() );
        box_->insertItem( translatedName );
        internalNames_[ translatedName ] = name;
    }
    else if( rule && name == current_.ascii() )
    {
        next_ = new NatureEditionCategory( parentWidget(), rule );
        next_->show();
        connect( next_, SIGNAL( NatureChanged( const QString& ) ), this, SLOT( OnNatureChanged( const QString& ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::OnNatureChanged
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::OnNatureChanged( const QString& nature )
{
    emit NatureChanged( current_ + "/" + nature );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::GetNature
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
QString NatureEditionCategory::GetNature() const
{
    QString result( current_ );
    if( next_ )
        result += "/" + next_->GetNature();
    return result;
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::SetNature
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::SetNature( const QString& nature )
{
    if( nature.isEmpty() )
    {
        Select( tools::translate( "models::app6", "undefined" ) );
        if( next_ )
            next_->SetNature( nature );
        return;
    }
    QStringList list = QStringList::split( "/", nature );
    Select( list.front() );
    list.pop_front();
    if( next_ && ! list.empty() )
    {
        QString newNature = list.join( "/" );
        next_->SetNature( newNature );
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::Select
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::Select( const QString& value )
{
    for( int i = 0; i < box_->count(); ++i )
        if( value == box_->text( i ) )
        {
            box_->setCurrentItem( i );
            OnComboChange();
            return;
        }
    // throw ?
}
