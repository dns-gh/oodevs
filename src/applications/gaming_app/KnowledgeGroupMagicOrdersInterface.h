// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupMagicOrdersInterface_h_
#define __KnowledgeGroupMagicOrdersInterface_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
    class KnowledgeGroup_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  KnowledgeGroupMagicOrdersInterface
@brief  KnowledgeGroupMagicOrdersInterface
*/
// Created: SLG 2009-12-17
// =============================================================================
class KnowledgeGroupMagicOrdersInterface : public QObject
    , public tools::Observer_ABC
    , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    KnowledgeGroupMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
    virtual ~KnowledgeGroupMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnActivateKnowledgeGroup();
    void OnDesactivateKnowledgeGroup();
    void OnSetType();
    void OnDeleteKnowledgeGroup();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroupMagicOrdersInterface( const KnowledgeGroupMagicOrdersInterface& );            //!< Copy constructor
    KnowledgeGroupMagicOrdersInterface& operator=( const KnowledgeGroupMagicOrdersInterface& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
