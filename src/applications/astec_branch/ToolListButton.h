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

// =============================================================================
/** @class  ToolListButtonBase
    @brief  ToolListButtonBase
*/
// Created: AGE 2006-03-27
// =============================================================================
class ToolListButtonBase : public QToolButton
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ToolListButtonBase( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent );
    explicit ToolListButtonBase( QToolBar* parent );
    virtual ~ToolListButtonBase();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ToolListButtonBase( const ToolListButtonBase& );            //!< Copy constructor
    ToolListButtonBase& operator=( const ToolListButtonBase& ); //!< Assignement operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemSelected( int );
    //@}

protected:
    //! @name Helpers
    //@{
    void AddItem( const QString& label, int i, bool selected = false );
    void Select( int i );
    virtual void OnSelected( int selected ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    QString toolTip_;
    QPopupMenu* menu_;
    int selected_;
    //@}
};

// =============================================================================
/** @class  ToolListButton
    @brief  ToolListButton
*/
// Created: AGE 2006-03-27
// =============================================================================
template< typename T >
class ToolListButton : public ToolListButtonBase, private Observer_ABC, public OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    ToolListButton( QToolBar* parent, Options& options, const std::string& option )
        : ToolListButtonBase( parent )
        , options_( options )
        , option_( option )
    {
        options_.Register( *this );
    }
    ToolListButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent, Options& options, const std::string& option )
        : ToolListButtonBase( iconSet, toolTip, parent )
        , options_( options )
        , option_( option )
    {}
    virtual ~ToolListButton()
    {};
    //@}

    //! @name Operations
    //@{
    void AddItem( const QString& label, const T& value, bool selected = false )
    {
        values_.push_back( value );
        ToolListButtonBase::AddItem( label, values_.size() - 1, selected );
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
        options_.Change( option_, values_.at( selected ) );
    };

    virtual void OptionChanged( const std::string& name, const OptionVariant& value )
    {
        if( name == option_ )
        {
            for( unsigned i = 0; i < values_.size(); ++i )
                if( values_[ i ] == value.To< T >() )
                {
                    Select( i );
                    return;
                }
            values_.push_back( value.To< T >() );
            Select( values_.size() - 1 );
        }
    };
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    std::string option_;
    std::vector< T > values_;
    //@}
};


#endif // __ToolListButton_h_
