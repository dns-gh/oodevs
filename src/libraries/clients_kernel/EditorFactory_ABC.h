// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditorFactory_ABC_h_
#define __EditorFactory_ABC_h_

#include "VirtualTemplate.h"

class QWidget;

namespace kernel
{

// =============================================================================
/** @class  EditorFactory_ABC
    @brief  EditorFactory_ABC
*/
// Created: SBO 2006-10-18
// =============================================================================
class EditorFactory_ABC : public VirtualTemplate< EditorFactory_ABC >
{
    friend VirtualTemplate< EditorFactory_ABC >;

public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory_ABC();
    virtual ~EditorFactory_ABC();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    QWidget* CreateEditor( QWidget* parent, const T& value )
    {
        BeginEditor( parent );
        VirtualTemplate< EditorFactory_ABC >::Call( value );
        return EndEditor();
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EditorFactory_ABC( const EditorFactory_ABC& );            //!< Copy constructor
    EditorFactory_ABC& operator=( const EditorFactory_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeginEditor( QWidget* parent ) = 0;
    virtual QWidget* EndEditor() = 0;

    template< typename T >
    void Call( const T& )
    {
        // NOTHING
    }
    //@}
};

}

#endif // __EditorFactory_ABC_h_
