// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "Schedulable_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "tools/Resolver_ABC.h"
#pragma warning( push, 0 )
#include <qstring.h>
#pragma warning( pop )

namespace sword
{
    class AutomatCreation;
}

namespace kernel
{
    class Mission;
    class LogisticLevel;
}

namespace mission_tester
{
    class Exercise;
// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: PHC 2011-03-28
// =============================================================================
class Automat : public Schedulable_ABC
              , public kernel::Automat_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const sword::AutomatCreation& message, const tools::Resolver_ABC< kernel::AutomatType >& resolver );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual const kernel::AutomatType& GetType() const;
    virtual bool Matches( const Filter_ABC& filter ) const;
    virtual bool Trigger( State_ABC& state );
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual const kernel::LogisticLevel& GetLogisticLevel( void ) const;

    virtual bool StartMission( Exercise& exercise );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const QString name_;
    const kernel::AutomatType& type_;
    const kernel::LogisticLevel& logisticLevel_;    
    std::auto_ptr< tools::Iterator_ABC< const kernel::Mission& > > current_;
    //@}
};
}

#endif // __Automat_h_
