// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_EditorFactory_h_
#define __gui_EditorFactory_h_

#include "EditorFactory_ABC.h"
#include "clients_kernel/SubTypes.h"

namespace gui
{
// =============================================================================
/** @class  EditorFactory
    @brief  Editor factory
*/
// Created: SBO 2006-10-18
// =============================================================================
class EditorFactory : public EditorFactory_ABC
                    , public tools::Caller< QString* >
                    , public tools::Caller< kernel::Duration* >
                    , public tools::Caller< QDateTime* >
                    , public tools::Caller< double* >
                    , public tools::Caller< float* >
                    , public tools::Caller< int* >
                    , public tools::Caller< unsigned int* >
                    , public tools::Caller< bool* >
                    , public tools::Caller< kernel::Unit* >
                    , public tools::Caller< unsigned long* >
{
public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory();
    virtual ~EditorFactory();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Call( QString* const& value );
    virtual void Call( kernel::Duration* const& value );
    virtual void Call( QDateTime* const& value );
    virtual void Call( double* const& value );
    virtual void Call( float* const& value );
    virtual void Call( int* const& value );
    virtual void Call( unsigned int* const& value );
    virtual void Call( bool* const& value );
    virtual void Call( kernel::Unit* const& value );
    virtual void Call( unsigned long* const& value );

    virtual void BeginEditor( QWidget* parent );
    virtual QWidget* EndEditor();
    //@}

protected:
    //! @name Member data
    //@{
    QWidget* parent_;
    QWidget* result_;

    kernel::Unit* unit_;
    //@}
};

}

#endif // __gui_EditorFactory_h_
