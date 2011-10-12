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

#include "clients_kernel/Infrastructure_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class PropertiesDictionary;
    class InfrastructureType;
}

namespace gui
{
    class TerrainObjectProxy;
}

// =============================================================================
/** @class  InfrastructureAttribute
    @brief  InfrastructureAttribute
*/
// Created: JSR 2010-09-01
// =============================================================================
class InfrastructureAttribute : public kernel::Infrastructure_ABC
                              , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InfrastructureAttribute( kernel::Controllers& controllers, gui::TerrainObjectProxy& object, const tools::StringResolver< kernel::InfrastructureType >& resolver, kernel::PropertiesDictionary& dictionary );
    virtual ~InfrastructureAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEnabled() const;
    virtual unsigned int GetThreshold() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
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
    kernel::Controllers& controllers_;
    const tools::StringResolver< kernel::InfrastructureType >& resolver_;
    kernel::Entity_ABC& object_;
    const geometry::Point2f position_;
    bool enabled_;
    unsigned int threshold_;
    std::string role_;
    const kernel::InfrastructureType* type_;
    //@}
};

#endif // __InfrastructureAttribute_h_
