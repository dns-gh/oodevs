// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoDialog.h"
#include "moc_InfoDialog.cpp"

#include "clients_gui/LogisticBase.h"
#include "clients_kernel/Controllers.h"

#include <boost/optional.hpp>

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog_Base::InfoDialog_Base( kernel::Controllers& controllers,
                                  QWidget* parent,
                                  const QString& title,
                                  const T_LogisticBaseModeChecker& modeChecker )
    : QDialog( parent )
    , controllers_( controllers )
    , disconnected_( false )
    , baseTitle_( title )
    , modeChecker_( modeChecker )
    , selected_( controllers )
{
    setWindowTitle( title );
    QLayout* layout = new Q3VBoxLayout( this );
    layout->setMargin( 5 );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    box_ = new Q3VBox( this );
    layout->add( box_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog_Base::~InfoDialog_Base()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::sizeHint
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QSize InfoDialog_Base::sizeHint() const
{
    return QSize( 500, 600 );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::RootWidget
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QWidget* InfoDialog_Base::RootWidget()
{
    return box_;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::hideEvent
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoDialog_Base::hideEvent( QHideEvent* event )
{
    if( !disconnected_ )
        emit Closed();
    QDialog::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::OnToggle
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoDialog_Base::OnToggle( bool state )
{
    if( !disconnected_ )
    {
        disconnected_ = true;
        setVisible( state );
    }
    disconnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::SetEnabled
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoDialog_Base::SetEnabled( bool state )
{
    emit Disabled( !state );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::showEvent
// Created: LGY 2014-02-05
// -----------------------------------------------------------------------------
void InfoDialog_Base::showEvent( QShowEvent* event )
{
    emit Shown();
    QDialog::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoDialog_Base::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    SetEnabled( entity && ShouldDisplay( *entity ) );
    boost::optional< bool > manual = boost::none;
    if( entity && modeChecker_ )
        if( const auto* base = entity->Retrieve< gui::LogisticBase >() )
            if( base->IsBase() )
                manual = modeChecker_( *base );
    UpdateTitle( manual );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::NotifyUpdated
// Created: NPT 2013-05-16
// -----------------------------------------------------------------------------
void InfoDialog_Base::NotifyUpdated( const kernel::Entity_ABC* element )
{
    if( isVisible() )
        SetEnabled( element && ShouldDisplay( *element ) );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::NotifyUpdated
// Created: ABR 2014-03-04
// -----------------------------------------------------------------------------
void InfoDialog_Base::NotifyUpdated( const gui::LogisticBase& base )
{
    if( isVisible() &&
        selected_ &&
        selected_->GetId() == base.GetEntity().GetId() &&
        modeChecker_ )
        UpdateTitle( modeChecker_( base ) );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::UpdateTitle
// Created: ABR 2014-03-04
// -----------------------------------------------------------------------------
void InfoDialog_Base::UpdateTitle( const boost::optional< bool >& manual )
{
    if( manual )
        setWindowTitle( baseTitle_ + " - " + ( *manual ? tr( "Manual" )
                                                       : tr( "Automatic" ) ) );
    else
        setWindowTitle( baseTitle_ );
}
