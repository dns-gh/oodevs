// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisTypeResolver_h_
#define __DisTypeResolver_h_

#include "clients_kernel/ApproximativeMap.h"
#include "hla_plugin/EntityType.h"
#include <map>

namespace kernel
{
    class ComponentType;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  DisTypeResolver
    @brief  DisTypeResolver
*/
// Created: AGE 2008-04-04
// =============================================================================
class DisTypeResolver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DisTypeResolver( const std::string& mappingFile );
    virtual ~DisTypeResolver();
    //@}

    //! @name Operations
    //@{
    hla::EntityType Find( const kernel::ComponentType& component ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisTypeResolver( const DisTypeResolver& );            //!< Copy constructor
    DisTypeResolver& operator=( const DisTypeResolver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadEntry( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::ApproximativeMap< hla::EntityType > types_;
    mutable std::map< const kernel::ComponentType*, hla::EntityType > resolved_;
    hla::EntityType default_;
    //@}
};

}
}

#endif // __DisTypeResolver_h_
