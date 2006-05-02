// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ToolListButton_h_
#define __ToolListButton_h_

#include <qtoolbutton.h>
#include "Options.h"
#include "Observer_ABC.h"
#include "OptionsObserver_ABC.h"
#include "OptionMenu.h"

// =============================================================================
/** @class  ToolListButton
    @brief  ToolListButton
*/
// Created: AGE 2006-03-27
// =============================================================================
template< typename T >
class ToolListButton : public QToolButton, private Observer_ABC, public OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    ToolListButton( QToolBar* parent, Options& options, const std::string& option )
        : QToolButton( parent )
        , options_( options )
        , option_( option )
        , menu_( new OptionMenu< T >( this, options, option ) )
    {
        setPopup( menu_ );
        setPopupDelay( 1 );
        setUsesTextLabel( true );
        setTextPosition( QToolButton::BesideIcon );
        options_.Register( *this );
    }

    ToolListButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent, Options& options, const std::string& option )
        : QToolButton( iconSet, "", "", 0, "", parent, "" )
        , options_( options )
        , option_( option )
        , menu_( new OptionMenu< T >( this, options, option ) )
        , toolTip_   ( toolTip )
    {
        setPopup( menu_ );
        setPopupDelay( 1 );
        QToolTip::add( this, toolTip_ );
        setUsesTextLabel( true );
        setTextPosition( QToolButton::BesideIcon );
        options_.Register( *this );
    }
    virtual ~ToolListButton()
    {
        options_.Remove( *this );
    };
    //@}

    //! @name Operations
    //@{
    void AddItem( const QString& label, const T& value )
    {
        menu_->AddItem( label, value );
        QToolTip::add( this, toolTip_ );
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ToolListButton( const ToolListButton& );            //!< Copy constructor
    ToolListButton& operator=( const ToolListButton& ); //!< Assignement operator
    //@}

    //! @name Operations
    //@{
    virtual void OnSelected( int selected )
    {
        menu_->OnSelected( selected );
    }

    virtual void OptionChanged( const std::string& name, const OptionVariant& value )
    {
        if( option_ == name )
        {
            menu_->OptionChanged( name, value );
            setTextLabel( menu_->text( menu_->selected_ ) );
            QToolTip::add( this, toolTip_ );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    std::string option_;
    OptionMenu< T >* menu_;
    QString toolTip_;
    //@}
};


#endif // __ToolListButton_h_
