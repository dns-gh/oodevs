// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanExportManager_h_
#define __UrbanExportManager_h_

#include <boost/noncopyable.hpp>

class PointProjector_ABC;
class UrbanModel;

// =============================================================================
/** @class  UrbanExportManager
    @brief  UrbanExportManager
*/
// Created: CMA 2012-03-12
// =============================================================================
class UrbanExportManager : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanExportManager( const std::string& directory, PointProjector_ABC& projector, const UrbanModel& model );
    virtual ~UrbanExportManager();
    //@}

    //! @name Operations
    //@{
    void Run() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string   directory_;
    const std::string   name_;
    PointProjector_ABC& projector_;
    const UrbanModel&   model_;
    //@}
};

#endif // __UrbanExportManager_h_
