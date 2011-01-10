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

namespace gui
{
    class TerrainObjectProxy;
}

namespace sword
{
    class UrbanCreation;
    class UrbanUpdate;
    class ObjectUpdate;
}

namespace urban
{
    class Model;
}

namespace kernel
{
    class Controllers;
    class DetectionMap;
}

class Model;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel, const kernel::DetectionMap& map );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::UrbanCreation& message );
    void Update( const sword::UrbanUpdate& message );
    void Update( const sword::ObjectUpdate& message );

    void Purge();
    const urban::Model& GetModel() const;
    const UrbanBlockDetectionMap& GetUrbanBlockMap() const;
    gui::TerrainObjectProxy& GetObject( unsigned long id ) const;
    gui::TerrainObjectProxy* FindObject( unsigned long id ) const;

    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Model& model_;
    const StaticModel& static_;
    std::auto_ptr< urban::Model > urbanModel_;
    const kernel::DetectionMap& map_;
    UrbanBlockDetectionMap& urbanBlockDetectionMap_;
    //@}
};

#endif // __UrbanModel_h_
