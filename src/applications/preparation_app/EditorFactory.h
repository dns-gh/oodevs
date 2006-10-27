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

#include "clients_gui/EditorFactory.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "preparation/Types.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Entity_ABC;
}

class Model;

// =============================================================================
/** @class  EditorFactory
    @brief  EditorFactory
*/
// Created: SBO 2006-10-25
// =============================================================================
class EditorFactory : public gui::EditorFactory
                    , public kernel::Caller< TC2* >
                    , public kernel::Caller< MaintenanceSuperior* >
                    , public kernel::Caller< MedicalSuperior* >
                    , public kernel::Caller< SupplySuperior* >
                    , public kernel::Observer_ABC
                    , public kernel::SelectionObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory( kernel::Controllers& controllers, Model& model );
    virtual ~EditorFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Call( TC2* const& value );
    virtual void Call( MaintenanceSuperior* const& value );
    virtual void Call( MedicalSuperior* const& value );
    virtual void Call( SupplySuperior* const& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EditorFactory( const EditorFactory& );            //!< Copy constructor
    EditorFactory& operator=( const EditorFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void BuildLogisticEditor( T& value );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __EditorFactory_h_
