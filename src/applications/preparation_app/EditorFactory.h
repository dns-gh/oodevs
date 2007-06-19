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
#include "clients_kernel/Resolver.h"
#include "preparation/Types.h"
#include "preparation/EnumTypes.h"

namespace kernel
{
    class Controllers;
    class Automat_ABC;
    class Entity_ABC;
    class NBCAgent;
    class KnowledgeGroupType;
    class Karma;
}

class Model;
class StaticModel;
class DotationsItem;

// =============================================================================
/** @class  EditorFactory
    @brief  EditorFactory
*/
// Created: SBO 2006-10-25
// =============================================================================
class EditorFactory : public gui::EditorFactory
                    , public kernel::Observer_ABC
                    , public kernel::SelectionObserver< kernel::Entity_ABC >
                    , public kernel::Caller< TC2* >
                    , public kernel::Caller< MaintenanceSuperior* >
                    , public kernel::Caller< MedicalSuperior* >
                    , public kernel::Caller< SupplySuperior* >
                    , public kernel::Caller< kernel::KnowledgeGroupType** >
                    , public kernel::Caller< kernel::Karma** >
                    , public kernel::Caller< kernel::NBCAgent** >
                    , public kernel::Caller< std::vector< kernel::NBCAgent* >* >
                    , public kernel::Caller< Enum_PopulationAttitude* >
                    , public kernel::Caller< Enum_ObstacleType* >
                    , public kernel::Caller< DotationsItem** >
{

public:
    //! @name Constructors/Destructor
    //@{
             EditorFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~EditorFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Call( TC2* const& value );
    virtual void Call( MaintenanceSuperior* const& value );
    virtual void Call( MedicalSuperior* const& value );
    virtual void Call( SupplySuperior* const& value );
    virtual void Call( kernel::KnowledgeGroupType** const& value );
    virtual void Call( kernel::Karma** const& value );
    virtual void Call( kernel::NBCAgent** const& value );
    virtual void Call( std::vector< kernel::NBCAgent* >* const& value );
    virtual void Call( Enum_PopulationAttitude* const& value );
    virtual void Call( Enum_ObstacleType* const& value );
    virtual void Call( DotationsItem** const& value );
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
    const StaticModel& staticModel_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __EditorFactory_h_
