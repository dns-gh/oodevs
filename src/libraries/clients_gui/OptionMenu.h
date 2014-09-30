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

#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include <tools/Observer_ABC.h>

namespace gui
{

// =============================================================================
/** @class  OptionMenuBase
    @brief  OptionMenuBase
*/
// Created: SBO 2006-04-28
// =============================================================================
class OptionMenuBase : public kernel::ContextMenu
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
class OptionMenu : public OptionMenuBase
                 , public tools::Observer_ABC
                 , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    OptionMenu( QWidget* parent, kernel::OptionsController& options, const std::string& option, bool savable = true )
        : OptionMenuBase( parent )
        , options_( options )
        , option_( option )
        , savable_( savable )
    {
        options_.Register( *this );
    }

    virtual ~OptionMenu()
    {
        options_.Unregister( *this );
    };
    //@}

    //! @name Operations
    //@{
    int AddItem( const QString& label, const T& value )
    {
        std::vector< T >::iterator it = std::find( values_.begin(), values_.end(), value );
        if( it != values_.end() )
            return static_cast< int >( std::distance( values_.begin(), it ) );

        values_.push_back( value );
        int id = static_cast< int >( values_.size() - 1 );
        OptionMenuBase::AddItem( label, id );
        return id;
    };
    //@}

public:
    //! @name Operations
    //@{
    virtual void OnSelected( int selected )
    {
        const T& value = values_.at( selected );
        options_.Change( option_, value, savable_ );
    };

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value )
    {
        if( name == option_ )
        {
            for( unsigned i = 0; i < values_.size(); ++i )
                if( values_[ i ] == value.To< T >() )
                {
                    Select( i );
                    return;
                }
        }
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    std::string option_;
    std::vector< T > values_;
    bool savable_;
    //@}
};

}

#endif // __OptionMenu_h_
