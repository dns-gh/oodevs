// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Entity_h_
#define __Entity_h_

#include "Schedulable_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "tools/Resolver_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace sword
{
    class UnitCreation;
}

namespace kernel
{
    class FragOrder;
    class Mission;
    class DecisionalModel;
}

namespace mission_tester
{
    class Exercise;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: PHC 2011-03-28
// =============================================================================
class Entity : public Schedulable_ABC
             , public kernel::Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Entity( const unsigned long id, const QString& name, const kernel::DecisionalModel& decisionalModel );
    virtual ~Entity();
    //@}

public:
    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual bool Matches( const Filter_ABC& filter ) const;
    virtual bool Start( Exercise& exercise, bool withFragOrders );
    bool StartMission( Exercise& exercise );
    bool StartFragOrder( Exercise& exercise );
    //bool Recomplete( Exercise& exercise );

    virtual QString GetTypeName() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const QString name_;
    unsigned int recompletionCountDown_;
    std::auto_ptr< tools::Iterator_ABC< const kernel::Mission& > > currentMission_;
    std::auto_ptr< tools::Iterator_ABC< const kernel::FragOrder& > > currentFragOrder_;
    //@}
};

}

#endif // __Entity_h_
