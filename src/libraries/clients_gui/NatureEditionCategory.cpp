// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
NatureEditionCategory::NatureEditionCategory( QGridLayout* parentLayout, int row, int deep /*= -1*/ )
    : QObject( parentLayout )
    , parentLayout_( parentLayout )
    , deep_        ( deep )
    , row_         ( row )
    , rule_        ( 0 )
    , current_     ( QString::null )
    , next_        ( 0 )
{
    label_ = new QLabel();
    box_ = new QComboBox();

    parentLayout->addWidget( label_, row_, 0 );
    parentLayout->addWidget( box_, row_, 1 );

    connect( box_, SIGNAL( activated( int ) ), this, SLOT( OnComboChange() ) );
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory destructor
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
NatureEditionCategory::~NatureEditionCategory()
{
    delete label_;
    delete box_;
    delete next_;
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::Clear
// Created: ABR 2011-11-02
// -----------------------------------------------------------------------------
void NatureEditionCategory::Clear()
{
    if( next_ )
        next_->Clear();
    delete next_;
    next_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::OnComboChange
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::OnComboChange()
{
    assert( rule_ );
    const QString current = box_->currentText();
    if( tools::findTranslation( "models::app6", current_ ) != current )
    {
        current_ = internalNames_[current].c_str();
        Clear();
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
        QString label = tools::findTranslation( "models::app6", title.c_str() );
        if( !label.isEmpty() )
        {
            QString tmp = label[ 0 ];
            label[ 0 ] = tmp[ 0 ].toUpper();
        }
        label = label + ":";
        label_->setText( label );
        box_->clear();
        internalNames_.clear();
        internalNames_[ tools::findTranslation( "models::app6", "undefined" ) ] = "undefined";
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
        QString translatedName = tools::findTranslation( "models::app6", name.c_str() );
        internalNames_[ translatedName ] = name;
    }
    else if( rule && name == current_.ascii() && deep_ != 0 )
    {
        next_ = new NatureEditionCategory( parentLayout_, row_ + 1, ( deep_ < 0 ) ? -1 : deep_ - 1 );
        next_->SetRootSymbolRule( *rule );
        connect( next_, SIGNAL( NatureChanged( const QString& ) ), this, SLOT( OnNatureChanged( const QString& ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::EndCategory
// Created: LDC 2010-10-04
// -----------------------------------------------------------------------------
void NatureEditionCategory::EndCategory()
{
    if( current_.isNull() )
    {
        box_->clear();
        for( std::map< QString, std::string >::const_iterator it = internalNames_.begin(); it != internalNames_.end(); ++it )
            box_->insertItem( it->first );
        Select( tools::findTranslation( "models::app6", "undefined" ) );
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
        Select( tools::findTranslation( "models::app6", "undefined" ) );
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
    else if( next_ )
    {
        QString empty;
        next_->SetNature( empty );
    }
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::Select
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
void NatureEditionCategory::Select( const QString& value )
{
    QString translatedValue = tools::findTranslation( "models::app6",  value );
    for( int i = 0; i < box_->count(); ++i )
    {
        if( translatedValue == box_->text( i ) )
        {
            box_->setCurrentItem( i );
            OnComboChange();
            return;
        }
    }
    // throw ?
}

// -----------------------------------------------------------------------------
// Name: NatureEditionCategory::SetRootSymbolRule
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void NatureEditionCategory::SetRootSymbolRule( kernel::SymbolRule& root )
{
    rule_ = &root;
    rule_->Accept( *this );
}
