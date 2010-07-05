// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EditorFactory_ABC_h_
#define __EditorFactory_ABC_h_

#include "tools/VirtualTemplate.h"
#include "Units.h"

class QWidget;

namespace kernel
{

// =============================================================================
/** @class  EditorFactory_ABC
    @brief  EditorFactory_ABC
*/
// Created: SBO 2006-10-18
// =============================================================================
class EditorFactory_ABC : public tools::VirtualTemplate< EditorFactory_ABC >
{
    friend tools::VirtualTemplate< EditorFactory_ABC >;

public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory_ABC();
    virtual ~EditorFactory_ABC();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    QWidget* CreateEditor( QWidget* parent, T* value )
    {
        BeginEditor( parent );
        tools::VirtualTemplate< EditorFactory_ABC >::Call( value );
        return EndEditor();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EditorFactory_ABC( const EditorFactory_ABC& );            //!< Copy constructor
    EditorFactory_ABC& operator=( const EditorFactory_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeginEditor( QWidget* parent ) = 0;
    virtual QWidget* EndEditor() = 0;

    template< typename T >
    void Call( T* )
    {
        // NOTHING
    }

    template< typename T >
    void Call( kernel::UnitedValue< T >* value )
    {
        tools::VirtualTemplate< EditorFactory_ABC >::Call( &value->unit_ );
        tools::VirtualTemplate< EditorFactory_ABC >::Call( &value->value_ );
    }
    //@}
};

}

#endif // __EditorFactory_ABC_h_
