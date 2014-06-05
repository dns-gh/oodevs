// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gaming_UrbanModel_h_
#define __Gaming_UrbanModel_h_

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class UrbanCreation;
    class UrbanUpdate;
    class ObjectUpdate;
}

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

namespace gui
{
    class UrbanDisplayOptions;
}

class ResourceNetworkModel;
class StaticModel;

// =============================================================================
/** @class  UrbanModel
    @brief  Urban model
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::UrbanObject_ABC >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, ResourceNetworkModel& resourceNetwork, const StaticModel& staticModel );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::UrbanCreation& message );
    void Update( const sword::UrbanUpdate& message );
    void Update( const sword::ObjectUpdate& message );

    void Purge();
    kernel::UrbanObject_ABC& GetObject( unsigned long id ) const;
    kernel::UrbanObject_ABC* FindObject( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ResourceNetworkModel& resourceNetwork_;
    const StaticModel& static_;
    std::unique_ptr< gui::UrbanDisplayOptions > urbanDisplayOptions_;
    //@}
};

#endif // __UrbanModel_h_
