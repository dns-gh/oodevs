// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditorFactory_h_
#define __EditorFactory_h_

#include "clients_kernel/EditorFactory_ABC.h"

namespace gui
{

// =============================================================================
/** @class  EditorFactory
    @brief  EditorFactory
*/
// Created: SBO 2006-10-18
// =============================================================================
class EditorFactory : public kernel::EditorFactory_ABC
                    , public kernel::Caller< QString* >
                    , public kernel::Caller< double* >
                    , public kernel::Caller< float* >
                    , public kernel::Caller< int* >
                    , public kernel::Caller< unsigned int* >
                    , public kernel::Caller< bool* >
{

public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory();
    virtual ~EditorFactory();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EditorFactory( const EditorFactory& );            //!< Copy constructor
    EditorFactory& operator=( const EditorFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( QString* const& value );
    virtual void Call( double* const& value );
    virtual void Call( float* const& value );
    virtual void Call( int* const& value );
    virtual void Call( unsigned int* const& value );
    virtual void Call( bool* const& value );

    virtual void BeginEditor( QWidget* parent );
    virtual QWidget* EndEditor();
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    QWidget* result_;
    //@}
};

}

#endif // __EditorFactory_h_
