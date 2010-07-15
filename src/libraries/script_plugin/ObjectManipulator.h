// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ObjectManipulator_h_
#define __ObjectManipulator_h_

#include "clients_kernel/Extension_ABC.h"
#include "dispatcher/Registrable_ABC.h"

namespace dispatcher
{
    class Object_ABC;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  ObjectManipulator
    @brief  Object manipulator
*/
// Created: SBO 2010-07-13
// =============================================================================
class ObjectManipulator : public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectManipulator( const dispatcher::Object_ABC& entity );
    virtual ~ObjectManipulator();
    //@}

    //! @name Operations
    //@{
    struct Registrar : public dispatcher::Registrable_ABC
    {
        virtual void RegisterIn( directia::Brain& brain );
    };
    void UsedByDIA() {}
    void ReleasedByDIA() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectManipulator( const ObjectManipulator& );            //!< Copy constructor
    ObjectManipulator& operator=( const ObjectManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned int GetIdentifier() const;
    std::string GetName() const;
    std::string GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Object_ABC& entity_;
    //@}
};

}
}

#endif // __ObjectManipulator_h_
