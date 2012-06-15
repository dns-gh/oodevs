// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StructuralStateAttribute_h_
#define __StructuralStateAttribute_h_

#include "clients_kernel/StructuralStateAttribute_ABC.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
    class Entity_ABC;
}

// =============================================================================
/** @class  StructuralStateAttribute
    @brief  StructuralStateAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class StructuralStateAttribute : public kernel::StructuralStateAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralStateAttribute( kernel::Entity_ABC& entity, kernel::Controller& controller, kernel::PropertiesDictionary& dictionary );
    virtual ~StructuralStateAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetValue() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;

    virtual void DoUpdate( const sword::UrbanUpdate& message );
    virtual void DoUpdate( const sword::UrbanCreation& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    unsigned int structuralState_;
    //@}
};

#endif // __StructuralStateAttribute_h_
