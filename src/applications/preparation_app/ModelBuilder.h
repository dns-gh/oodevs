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

#include <qobject.h>
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "preparation/Types.h"

namespace kernel
{
    class Controllers;
    class ContextMenu;
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Formation_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
}

class Model;

// =============================================================================
/** @class  ModelBuilder
    @brief  ModelBuilder
*/
// Created: SBO 2006-08-30
// =============================================================================
class ModelBuilder : public QObject
                   , public kernel::Observer_ABC
                   , public kernel::SelectionObserver_ABC
                   , public kernel::SelectionObserver_Base< kernel::Team_ABC >
                   , public kernel::SelectionObserver_Base< kernel::KnowledgeGroup_ABC >
                   , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
                   , public kernel::SelectionObserver_Base< kernel::Automat_ABC >
                   , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
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
    void ClearSelection();
    void CreateLimit( const T_PointVector& points );
    void CreateLima ( const T_PointVector& points, E_FuncLimaType type );
    //@}

public slots:
    //! @name Slots
    //@{
    bool OnDelete();
    void OnCreate();
    void OnCreateFormation( int level );
    void OnCreateCommunication();
    void OnRename( QListViewItem*, int, const QString& text );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelBuilder( const ModelBuilder& );            //!< Copy constructor
    ModelBuilder& operator=( const ModelBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Team_ABC& element );
    virtual void Select( const kernel::KnowledgeGroup_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );

    const kernel::Entity_ABC* FindTacticalLineSuperior() const;
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
    //@}
};

#endif // __ModelBuilder_h_
