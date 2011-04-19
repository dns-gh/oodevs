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

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace gui
{
    class TerrainObjectProxy;
    class UrbanDisplayOptions;
}

namespace sword
{
    class UrbanCreation;
    class UrbanUpdate;
    class ObjectUpdate;
}

namespace kernel
{
    class Controllers;
}

class ResourceNetworkModel;
class StaticModel;
class UrbanBlockDetectionMap;

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
    // $$$ FDS 2009-01-11: inherit urban::Model to remove private aggregated model
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< gui::TerrainObjectProxy >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, ResourceNetworkModel& resourceNetwork, const StaticModel& staticModel, UrbanBlockDetectionMap& map );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::UrbanCreation& message );
    void Update( const sword::UrbanUpdate& message );
    void Update( const sword::ObjectUpdate& message );

    void Purge();
    gui::TerrainObjectProxy& GetObject( unsigned long id ) const;
    gui::TerrainObjectProxy* FindObject( unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ResourceNetworkModel& resourceNetwork_;
    const StaticModel& static_;
    std::auto_ptr< gui::UrbanDisplayOptions > urbanDisplayOptions_;
    UrbanBlockDetectionMap& urbanBlockDetectionMap_;
    //@}
};

#endif // __UrbanModel_h_
