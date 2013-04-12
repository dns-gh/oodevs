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
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
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
             StructuralStateAttribute( kernel::Entity_ABC& entity, kernel::Controller& controller, gui::PropertiesDictionary& dictionary );
    virtual ~StructuralStateAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetValue() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary ) const;

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
    const QString property_;
    //@}
};

#endif // __StructuralStateAttribute_h_
