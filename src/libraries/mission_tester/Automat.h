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

#include "Entity.h"
#include "clients_kernel/Automat_ABC.h"
#include "tools/Resolver_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
    class AutomatType;
}

namespace sword
{
    class AutomatCreation;
}

namespace mission_tester
{
// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: PHC 2011-03-28
// =============================================================================
class Automat : public Entity
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
    virtual const kernel::AutomatType& GetType() const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;

    virtual bool Matches( const Filter_ABC& filter ) const;
    virtual QString GetTypeName() const;
    //@}

    //! @name Entity_ABC
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& type_;
    const kernel::LogisticLevel& logisticLevel_;
    //@}
};

}

#endif // __Automat_h_
