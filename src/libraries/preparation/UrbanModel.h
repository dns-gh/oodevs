// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_UrbanModel_h_
#define __preparation_UrbanModel_h_

#include "clients_kernel/UrbanDisplayOptions.h"
#include "spatialcontainer/SpatialContainer.h"
#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include <boost/noncopyable.hpp>

namespace geostore
{
    class GeoStoreManager;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Location_ABC;
    class Object_ABC;
    class ObjectTypes;
    class UrbanObject_ABC;
}

namespace tools
{
    class ExerciseConfig;
    class SchemaWriter_ABC;
    class IdManager;
}

class QProgressDialog;
class StaticModel;
class UrbanFactory_ABC;
class UrbanMenuManager;

// =============================================================================
/** @class  UrbanModel
    @brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanModel : public tools::Resolver< kernel::UrbanObject_ABC >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                 , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct QuadTreeTraits;
    typedef spatialcontainer::SpatialContainer< const kernel::UrbanObject_ABC*, QuadTreeTraits, float > T_QuadTree;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanModel( kernel::Controllers& controllers, const StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects, tools::IdManager& idManager );
    virtual ~UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load();
    void LoadUrban( xml::xistream& xis );
    void LoadUrbanState( xml::xistream& xis );
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void CreateCityOrDistrict( kernel::Entity_ABC* parent );
    void CreateUrbanBlocks( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, bool isAuto );
    kernel::UrbanObject_ABC* Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent );
    void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects );
    void DeleteBlock( const kernel::UrbanObject_ABC& urbanObject );
    void ExportShapeFile( const std::string& exportDirectory, const tools::ExerciseConfig& config, QProgressDialog& progressDialog ) const;
    UrbanMenuManager& GetUrbanMenuManager() const;
    bool TakeLinkErrors();
    //@}

    //! @name Accessors
    //@{
    const kernel::UrbanObject_ABC* FindBlock( const geometry::Point2f& center ) const;
    void GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< const kernel::UrbanObject_ABC* >& result ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    void ReadCity( xml::xistream& xis );
    void ReadDistrict( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadBlock( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadUrbanObject( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, kernel::UrbanObject_ABC& object );
    template< typename T, typename U >
    void UpdateCapacity( xml::xistream& xis, kernel::UrbanObject_ABC& object );
    void SerializeExercise( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void SerializeTerrain( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void CreateQuadTree( float width, float height );
    void InsertIntoQuadTree( const kernel::UrbanObject_ABC& urbanObject );
    void CleanLinks();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                            controllers_;
    const StaticModel&                              staticModel_;
    const tools::Resolver< kernel::Object_ABC >&    objects_;
    std::auto_ptr< kernel::UrbanDisplayOptions >    urbanDisplayOptions_;
    std::auto_ptr< UrbanFactory_ABC >               factory_;
    std::auto_ptr< geostore::GeoStoreManager >      geostore_;
    std::auto_ptr< T_QuadTree >                     quadTree_;
    std::auto_ptr< UrbanMenuManager >               menuManager_;
    float                                           precision_;
    float                                           maxElementSize_;
    bool                                            cleanedLinks_;
    //@}
};

#endif // __preparation_UrbanModel_h_
