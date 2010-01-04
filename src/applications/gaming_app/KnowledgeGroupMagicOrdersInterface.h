// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupMagicOrdersInterface_h_
#define __KnowledgeGroupMagicOrdersInterface_h_

#include "game_asn/Simulation.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
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
             KnowledgeGroupMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroupMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateSubKnowledgeGroup();
    void OnActivateKnowledgeGroup();
    void OnDesactivateKnowledgeGroup();
    void OnSetType( int );
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
    void AddMagicTypeItem( const QString& label, const char* slot, QPopupMenu* menu, const kernel::KnowledgeGroupType& type, int id );
    //@}

    //! @name Types
    //@{
    typedef std::map< int, const kernel::KnowledgeGroupType* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& knowledgeGroupTypes_;
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedEntity_;
    T_Items items_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
