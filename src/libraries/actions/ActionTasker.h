// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __actions_ActionTasker_h_
#define __actions_ActionTasker_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xostream;
}

namespace kernel
{
    class Entity_ABC;
}

namespace actions
{

// =============================================================================
/** @class  ActionTasker
    @brief  ActionTasker
*/
// Created: SBO 2010-05-03
// =============================================================================
class ActionTasker : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionTasker( const kernel::Entity_ABC* tasker, bool simulation = true );
    virtual ~ActionTasker();
    //@}

    //! @name Operations
    //@{
    bool IsSimulation() const;
    unsigned int GetTaskerId() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int taskerId_;
    const bool simulation_;
    //@}
};

}

#endif // __actions_ActionTasker_h_
