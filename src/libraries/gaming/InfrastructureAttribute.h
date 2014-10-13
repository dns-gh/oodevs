// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InfrastructureAttribute_h_
#define __InfrastructureAttribute_h_

#include "clients_gui/Infrastructure_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class InfrastructureType;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  InfrastructureAttribute
    @brief  InfrastructureAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class InfrastructureAttribute : public gui::Infrastructure_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfrastructureAttribute( kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, const tools::StringResolver< kernel::InfrastructureType >& resolver, gui::PropertiesDictionary& dictionary );
    virtual ~InfrastructureAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEnabled() const;
    virtual const std::string& GetInvalidType() const;
    virtual unsigned int GetThreshold() const;
    virtual const kernel::InfrastructureType* GetType() const;
    virtual void SetType( kernel::InfrastructureType* infrastructure );
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
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
    kernel::Controllers&                                       controllers_;
    const tools::StringResolver< kernel::InfrastructureType >& resolver_;
    kernel::Entity_ABC&                                        object_;
    const geometry::Point2f                                    position_;
    bool                                                       enabled_;
    unsigned int                                               threshold_;
    kernel::OptionalValue< std::string >                       role_;
    const kernel::InfrastructureType*                          type_;
    //@}
};

#endif // __InfrastructureAttribute_h_
