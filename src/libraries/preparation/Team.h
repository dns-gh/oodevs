// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
}

namespace xml
{
    class xostream;
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: SBO 2006-08-29
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
           , public kernel::Extension_ABC
           , public kernel::Serializable_ABC
           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( kernel::Controller& controller,IdManager& idManager );
             Team( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    void Rename( const QString& name );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}
};

#endif // __Team_h_
