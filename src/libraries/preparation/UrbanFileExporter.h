// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFileExporter_h_
#define __UrbanFileExporter_h_

#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )

namespace kernel
{
    class UrbanObject_ABC;
}

class PointProjector_ABC;
class UrbanModel;

// =============================================================================
/** @class  UrbanFileExporter
    @brief  UrbanFileExporter
*/
// Created: CMA 2012-03-12
// =============================================================================
class UrbanFileExporter : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanFileExporter( const std::string& directory, const std::string& name, PointProjector_ABC& projector, const UrbanModel& model );
    virtual ~UrbanFileExporter();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateStructure();
    void CreateField( const char* name, OGRFieldType type, int width, int precision = 0 );
    void Initialize();
    void Write();
    void WriteObject( const kernel::UrbanObject_ABC& object, unsigned int counter );
    //@}

private:
    //! @name Member data
    //@{
    const std::string   directory_;
    const std::string   name_;
    PointProjector_ABC& projector_;
    const UrbanModel&   urbanModel_;
    OGRDataSource*      source_;
    OGRLayer*           layer_;
    //@}
};

#endif // __UrbanFileExporter_h_
