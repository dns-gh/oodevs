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
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class ContextMenu;
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Agent_ABC;
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
                   , public kernel::ContextMenuObserver_ABC< Model >
                   , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
                   , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
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
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateTeam();
    void OnCreateKnowledgeGroup();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelBuilder( const ModelBuilder& );            //!< Copy constructor
    ModelBuilder& operator=( const ModelBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Model&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Team_ABC&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Agent_ABC&, kernel::ContextMenu& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const kernel::Team_ABC* currentTeam_;
    //@}
};

#endif // __ModelBuilder_h_
