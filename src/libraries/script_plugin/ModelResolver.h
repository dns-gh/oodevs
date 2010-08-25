// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelResolver_h_
#define __ModelResolver_h_

namespace kernel
{
    class Entity_ABC;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace script
{
    class CommandString;

// =============================================================================
/** @class  ModelResolver
    @brief  Model_ABC resolver
*/
// Created: AGE 2008-07-15
// =============================================================================
class ModelResolver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelResolver( const dispatcher::Model_ABC& model );
    virtual ~ModelResolver();
    //@}

    //! @name Operations
    //@{
    const kernel::Entity_ABC* Resolve( const CommandString& command );
    const kernel::Entity_ABC* Resolve( const std::string& type, const std::string& id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ModelResolver( const ModelResolver& );            //!< Copy constructor
    ModelResolver& operator=( const ModelResolver& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};

}
}

#endif // __ModelResolver_h_
