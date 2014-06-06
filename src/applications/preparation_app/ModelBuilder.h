// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelBuilder_h_
#define __ModelBuilder_h_

#include "clients_gui/ModelObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class AutomatType;
    class Controllers;
    class ContextMenu;
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Formation_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
    class Ghost_ABC;
    class Object_ABC;
    class UrbanObject_ABC;
}

class Model;

// =============================================================================
/** @class  ModelBuilder
    @brief  ModelBuilder
*/
// Created: SBO 2006-08-30
// =============================================================================
class ModelBuilder : public QObject
                   , public gui::ModelObserver_ABC
                   , public tools::Observer_ABC
                   , public tools::SelectionObserver_ABC
                   , public tools::SelectionObserver_Base< kernel::Team_ABC >
                   , public tools::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                   , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                   , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                   , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                   , public tools::SelectionObserver_Base< kernel::Ghost_ABC >
                   , public tools::SelectionObserver_Base< kernel::Object_ABC >
                   , public tools::SelectionObserver_Base< kernel::UrbanObject_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ModelBuilder( kernel::Controllers& controllers, Model& model );
    virtual ~ModelBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void OnRename( kernel::Entity_ABC& entity, const QString& newName );
    virtual void CreateCommunication();
    virtual void CreateTeam();
    virtual void ClearSelection();
    virtual void CreateFormation( int level );
    virtual kernel::Automat_ABC* ReplaceAutomat( kernel::Entity_ABC& original, const kernel::AutomatType& type );
    virtual void DeleteEntity( const kernel::Entity_ABC& entity );
    virtual void CreateCityOrDistrict( kernel::Entity_ABC* parent );
    virtual void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects );
    bool CanCreateLine() const;
    void CreateLimit( const T_PointVector& points );
    void CreateLima( const T_PointVector& points );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnConfirmDeletion( int result );
    void OnDelete();
    void OnCreate();
    void OnCreateFormation( int level );
    void OnCreateCommunication();
    //@}

private:
    //! @name Operations
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Team_ABC& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Ghost_ABC& element );
    virtual void Select( const kernel::Object_ABC& element );
    virtual void Select( const kernel::UrbanObject_ABC& element );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    void Delete( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    kernel::SafePointer< kernel::Team_ABC >           selectedTeam_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedGroup_;
    kernel::SafePointer< kernel::Agent_ABC >          selectedAgent_;
    kernel::SafePointer< kernel::Automat_ABC >        selectedAutomat_;
    kernel::SafePointer< kernel::Formation_ABC >      selectedFormation_;
    kernel::SafePointer< kernel::Ghost_ABC >          selectedGhost_;
    kernel::SafePointer< kernel::Object_ABC >         selectedObject_;
    kernel::SafePointer< kernel::UrbanObject_ABC >    selectedUrbanObject_;
    kernel::SafePointer< kernel::Entity_ABC >         toDelete_;
    std::unique_ptr< QMessageBox >                    confirmation_;
    const QString                                     property_;
    //@}
};

#endif // __ModelBuilder_h_
