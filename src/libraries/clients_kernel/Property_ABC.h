// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Property_ABC_h_
#define __Property_ABC_h_

#include "ValueEditor.h"
#include "Controller.h"
#include "EditorFactory_ABC.h"
#include "Displayer_ABC.h"
#pragma warning( push, 0 )
#include <QtGui/qwidget.h>
#pragma warning( pop )

namespace kernel
{
    class Displayer_ABC;

// =============================================================================
/** @class  Property_ABC
    @brief  Property_ABC
*/
// Created: SBO 2006-10-17
// =============================================================================
class Property_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Property_ABC() {}
    virtual ~Property_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory ) = 0;
    virtual void SetValueFromEditor( QWidget* editor ) = 0;
    virtual void Display( Displayer_ABC& displayer ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Property_ABC( const Property_ABC& );            //!< Copy constructor
    Property_ABC& operator=( const Property_ABC& ); //!< Assignment operator
    //@}
};

template< typename T, typename Owner, typename Setter >
class Property : public Property_ABC
{
public:
    Property( Controller& controller, const Owner& owner, T& value, const Setter& setter )
        : controller_( controller )
        , owner_( owner )
        , data_( &value )
        , setter_( setter ) {

    }
    virtual ~Property() {};

    virtual void Display( Displayer_ABC& displayer )
    {
        displayer.Display( *data_ );
    }

    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory )
    {
        return factory.CreateEditor( parent, data_ );
    }

    virtual void SetValueFromEditor( QWidget* editor )
    {
        ValueEditor< T >* ed = dynamic_cast< ValueEditor< T >* >( editor );
        if( ed )
        {
            setter_( data_, ed->GetValue() );
            controller_.Update( owner_ );
        }
    }

private:
    Controller& controller_;
    const Owner& owner_;
    T* data_;
    Setter setter_;
};

}

#endif // __Property_ABC_h_
