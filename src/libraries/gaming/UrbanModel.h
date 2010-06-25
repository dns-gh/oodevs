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

namespace MsgsSimToClient
{
    class MsgUrbanCreation;
}

namespace urban
{
    class Model;
}

namespace kernel
{
    class Controller;
    class DetectionMap;
}

class TerrainObjectProxy;
class UrbanBlockDetectionMap;

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
    // $$$ FDS 2009-01-11: inherit urban::Model to remove private aggregated model
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controller& controller, const kernel::DetectionMap& map ); 
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Create( const MsgsSimToClient::MsgUrbanCreation& message );
    void Purge();
    const urban::Model& GetModel() const;
    const UrbanBlockDetectionMap& GetUrbanBlockMap() const;
    kernel::Entity_ABC& GetObject( unsigned long id ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UrbanModel( const UrbanModel& );            //!< Copy constructor
    UrbanModel& operator=( const UrbanModel& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::auto_ptr< urban::Model > model_;
    const kernel::DetectionMap& map_;
    UrbanBlockDetectionMap& urbanBlockDetectionMap_;
    //@}
};

#endif // __UrbanModel_h_
