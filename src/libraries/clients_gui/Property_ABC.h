// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui_Property_ABC_h_
#define gui_Property_ABC_h_

#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"

#include "ValueEditor.h"
#include "EditorFactory_ABC.h"
#include <boost/optional.hpp>
#pragma warning( push, 0 )
#include <QtGui/qwidget.h>
#pragma warning( pop )

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;
}

namespace gui
{
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
    //! @name Types
    //@{
    typedef std::function< bool() > T_ReadOnlyFunctor;
    //@}

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
    virtual void Display( kernel::Displayer_ABC& displayer ) = 0;
    virtual const QString& GetName() const = 0;
    virtual E_Category GetCategory() const = 0;
    virtual void AddSubProperty( Property_ABC* property ) = 0;
    virtual const kernel::Entity_ABC& GetOwner() const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Property_ABC( const Property_ABC& );            //!< Copy constructor
    Property_ABC& operator=( const Property_ABC& ); //!< Assignment operator
    //@}
};

template< typename T, typename Setter, typename SpecificOwner >
class Property : public Property_ABC
{
public:
    Property( kernel::Controller& controller,
              const kernel::Entity_ABC& owner,
              T& value,
              const Setter& setter,
              const QString& name,
              bool readOnly,
              E_Category category,
              const SpecificOwner* pSpecificOwner = nullptr )
        : controller_( controller )
        , owner_     ( owner )
        , data_      ( &value )
        , setter_    ( setter )
        , name_      ( name )
        , category_  ( category )
        , readOnly_  ( readOnly )
        , specificOwner_( pSpecificOwner )
    {
        // NOTHING
    }
    Property( kernel::Controller& controller,
              const kernel::Entity_ABC& owner,
              T& value,
              const Setter& setter,
              const QString& name,
              const T_ReadOnlyFunctor& readOnlyFunctor,
              E_Category category,
              const SpecificOwner* pSpecificOwner = nullptr )
        : controller_( controller )
        , owner_( owner )
        , data_( &value )
        , setter_( setter )
        , name_( name )
        , category_( category )
        , readOnlyFunctor_( readOnlyFunctor )
        , specificOwner_( pSpecificOwner )
    {
        // NOTHING
    }
    virtual ~Property()
    {
        // NOTHING
    }

    virtual void Display( kernel::Displayer_ABC& displayer )
    {
        displayer.Display( *data_ );
    }

    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory )
    {
        if( readOnly_ && *readOnly_ || readOnlyFunctor_ && readOnlyFunctor_() )
            return 0;
        return factory.CreateEditor( parent, data_ );
    }

    virtual void SetValueFromEditor( QWidget* editor )
    {
        ValueEditor< T >* ed = dynamic_cast< ValueEditor< T >* >( editor );
        if( ed )
        {
            setter_( data_, ed->GetValue() );
            if( specificOwner_ )
                controller_.Update( *specificOwner_ );
            else
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

    virtual const kernel::Entity_ABC& GetOwner() const
    {
        return owner_;
    }

private:
    kernel::Controller& controller_;
    const kernel::Entity_ABC& owner_;
    const SpecificOwner* specificOwner_;
    T* data_;
    Setter setter_;
    boost::optional< bool > readOnly_;
    T_ReadOnlyFunctor readOnlyFunctor_;
    const QString name_;
    E_Category category_;
};

}

#endif // gui_Property_ABC_h_
