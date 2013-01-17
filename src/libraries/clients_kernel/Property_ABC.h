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

    enum E_Category
    {
        eNothing,
        ePopulationRepartition,
        eUrbanTemplate
    };

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
    virtual const QString& GetName() const = 0;
    virtual E_Category GetCategory() const = 0;
    virtual void AddSubProperty( Property_ABC* property ) = 0;
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
    Property( Controller& controller, const Owner& owner, T& value, const Setter& setter,
              const QString& name, bool readOnly, E_Category category )
        : controller_( controller )
        , owner_     ( owner )
        , data_      ( &value )
        , setter_    ( setter )
        , name_      ( name )
        , category_  ( category )
        , readOnly_  ( readOnly )
    {
        // NOTHING
    }
    virtual ~Property()
    {
        // NOTHING
    }

    virtual void Display( Displayer_ABC& displayer )
    {
        displayer.Display( *data_ );
    }

    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory )
    {
        if( readOnly_ )
            return 0;
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

    virtual const QString& GetName() const
    {
        return name_;
    }

    virtual E_Category GetCategory() const
    {
        return category_;
    }

    virtual void AddSubProperty( Property_ABC* /*property*/ )
    {
        // NOTHING
    }

private:
    Controller& controller_;
    const Owner& owner_;
    T* data_;
    Setter setter_;
    bool readOnly_;
    const QString name_;
    E_Category category_;
};

}

#endif // __Property_ABC_h_
