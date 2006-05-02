// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionMenu_h_
#define __OptionMenu_h_

#include "Options.h"
#include "Observer_ABC.h"
#include "OptionsObserver_ABC.h"

// =============================================================================
/** @class  OptionMenuBase
    @brief  OptionMenuBase
*/
// Created: SBO 2006-04-28
// =============================================================================
class OptionMenuBase : public QPopupMenu
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
    explicit OptionMenuBase( QWidget* parent );
    virtual ~OptionMenuBase();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemSelected( int id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    OptionMenuBase( const OptionMenuBase& );            //!< Copy constructor
    OptionMenuBase& operator=( const OptionMenuBase& ); //!< Assignement operator
    //@}

public:
    //! @name Helpers
    //@{
    void AddItem( const QString& label, int i );
    void Select( int i );
    virtual void OnSelected( int selected ) = 0;
    //@}

public:
    //! @name Member data
    //@{
    int selected_;
    //@}
};

// =============================================================================
/** @class  OptionMenu
    @brief  OptionMenu
*/
// Created: SBO 2006-04-28
// =============================================================================
template< typename T >
class OptionMenu : public OptionMenuBase, private Observer_ABC, public OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    OptionMenu( QWidget* parent, Options& options, const std::string& option )
        : OptionMenuBase( parent )
        , options_( options )
        , option_( option )
    {
        options_.Register( *this );
    }
    
    virtual ~OptionMenu()
    {
        options_.Remove( *this );
    };
    //@}

    //! @name Operations
    //@{
    void AddItem( const QString& label, const T& value )
    {
        values_.push_back( value );
        OptionMenuBase::AddItem( label, values_.size() - 1 );
    };
    //@}

public:
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
    //! @name Copy/Assignement
    //@{
    OptionMenu( const OptionMenu& );            //!< Copy constructor
    OptionMenu& operator=( const OptionMenu& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    std::string option_;
    std::vector< T > values_;
    //@}
};

#endif // __OptionMenu_h_
