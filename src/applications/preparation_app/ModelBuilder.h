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

#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class AutomatType;
    class Controllers;
    class ContextMenu;
    class Inhabitant_ABC;
    class Population_ABC;
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
                   , public tools::Observer_ABC
                   , public tools::SelectionObserver_ABC
                   , public tools::SelectionObserver_Base< kernel::Team_ABC >
                   , public tools::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                   , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                   , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                   , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                   , public tools::SelectionObserver_Base< kernel::Ghost_ABC >
                   , public tools::SelectionObserver_Base< kernel::Inhabitant_ABC >
                   , public tools::SelectionObserver_Base< kernel::Object_ABC >
                   , public tools::SelectionObserver_Base< kernel::Population_ABC >
                   , public tools::SelectionObserver_Base< kernel::UrbanObject_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ModelBuilder( kernel::Controllers& controllers, Model& model );
    virtual ~ModelBuilder();
    //@}

    //! @name Operations
    //@{
    void ClearSelection();
    bool CanCreateLine() const;
    void CreateLimit( const T_PointVector& points );
    void CreateLima( const T_PointVector& points );
    void CreateCityOrDistrict( kernel::Entity_ABC* parent );
    void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects );
    void DeleteEntity( const kernel::Entity_ABC& entity );
    kernel::Automat_ABC* ReplaceAutomat( kernel::Entity_ABC& original, const kernel::AutomatType& type );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnConfirmDeletion( int result );
    void OnDelete();
    void OnCreate();
    void OnCreateFormation( int level );
    void OnCreateCommunication();
    void OnRename( Q3ListViewItem*, int, const QString& text );
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
    virtual void Select( const kernel::Inhabitant_ABC& element );
    virtual void Select( const kernel::Object_ABC& element );
    virtual void Select( const kernel::Population_ABC& element );
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
    kernel::SafePointer< kernel::Inhabitant_ABC >     selectedInhabitant_;
    kernel::SafePointer< kernel::Object_ABC >         selectedObject_;
    kernel::SafePointer< kernel::Population_ABC >     selectedPopulation_;
    kernel::SafePointer< kernel::UrbanObject_ABC >    selectedUrbanObject_;
    kernel::SafePointer< kernel::Entity_ABC >         toDelete_;
    std::auto_ptr< QMessageBox >                      confirmation_;
    //@}
};

#endif // __ModelBuilder_h_
