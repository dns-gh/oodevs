// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LongNameEditor_h_
#define __LongNameEditor_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class AttributeType;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class ExtensionTypes;
    class Formation_ABC;
    class Team_ABC;
}

class StaticModel;

// =============================================================================
/** @class  LongNameEditor
    @brief  LongNameEditor
*/
// Created: JSR 2011-09-12
// =============================================================================
class LongNameEditor : public QObject
                     , public tools::Observer_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                     , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LongNameEditor( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model );
    virtual ~LongNameEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Generate();
    //@}

private:
    //! @name Helpers
    //@{
    void DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    std::string GetCountryAlias( const std::string& country ) const;
    void RemoveCountryEnding( const kernel::Entity_ABC& entity, QString& name ) const;
    void TransmitToSubordinates( const kernel::Entity_ABC& entity, const QString& name, const kernel::AttributeType& attribute ) const;
    void SetExtension( kernel::Entity_ABC& entity, const QString& name, const kernel::AttributeType& attribute ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ExtensionTypes& types_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __LongNameEditor_h_
