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

#include "clients_kernel/Options.h"
#include "tools/Observer_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "OptionMenu.h"

#pragma warning( push, 0 )
#include <QtGui/qtoolbutton.h>
#pragma warning( pop )
namespace gui
{

// =============================================================================
/** @class  ToolListButtonBase
    @brief  ToolListButtonBase
*/
// Created: AGE 2006-03-27
// =============================================================================
class ToolListButtonBase : public QToolButton
{
public:
    //! @name Constructors/Destructor
    //@{
    ToolListButtonBase( Q3ToolBar* parent )
        : QToolButton( parent )
    {
        setUsesTextLabel( true );
        setTextPosition( QToolButton::BesideIcon );
    }

    ToolListButtonBase( const QIcon& iconSet, const QString& toolTip, Q3ToolBar* parent )
        : QToolButton( iconSet, "", "", 0, "", parent, "" )
        , toolTip_   ( toolTip )
    {
        QToolTip::add( this, toolTip_ );
        setUsesTextLabel( true );
        setTextPosition( QToolButton::BesideIcon );
    }

    virtual ~ToolListButtonBase()
    {
        // NOTHING
    };
    //@}

protected:
    //! @name
    //@{
    void SetToolTip()
    {
        QToolTip::add( this, toolTip_ );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ToolListButtonBase( const ToolListButtonBase& );            //!< Copy constructor
    ToolListButtonBase& operator=( const ToolListButtonBase& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QString toolTip_;
    //@}
};

// =============================================================================
/** @class  ToolListButton
    @brief  ToolListButton
*/
// Created: AGE 2006-03-27
// =============================================================================
template< typename T >
class ToolListButton : public ToolListButtonBase, public tools::Observer_ABC, public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ToolListButton( Q3ToolBar* parent, kernel::Options& options, const std::string& option )
        : ToolListButtonBase( parent )
        , options_( options )
        , option_( option )
        , menu_( new OptionMenu< T >( this, options, option ) )
    {
        setPopup( menu_ );
        setPopupDelay( 1 );
        options_.Register( *this );
    }

    ToolListButton( const QIcon& iconSet, const QString& toolTip, Q3ToolBar* parent, kernel::Options& options, const std::string& option )
        : ToolListButtonBase( iconSet, toolTip, parent )
        , options_( options )
        , option_( option )
        , menu_( new OptionMenu< T >( this, options, option ) )
    {
        setPopup( menu_ );
        setPopupDelay( 1 );
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
        SetToolTip();
    };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ToolListButton( const ToolListButton& );            //!< Copy constructor
    ToolListButton& operator=( const ToolListButton& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void OnSelected( int selected )
    {
        menu_->OnSelected( selected );
    }

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value )
    {
        if( option_ == name )
        {
            menu_->OptionChanged( name, value );
            setTextLabel( menu_->text( menu_->selected_ ) );
            SetToolTip();
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    kernel::Options& options_;
    std::string option_;
    OptionMenu< T >* menu_;
    //@}
};

}

#endif // __ToolListButton_h_
