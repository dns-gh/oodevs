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

#include "clients_gui/UrbanDisplayOptions.h"
#include "spatialcontainer/SpatialContainer.h"
#include "geostore/SpatialIndexer.h"
#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace geostore
{
    class Geostore;
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
                 , public geostore::SpatialIndexer
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
    void Serialize( const tools::Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void CreateCityOrDistrict( kernel::Entity_ABC* parent );
    void CreateAutoUrbanBlocks( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, double roadWidth );
    void CreateManualUrbanBlock( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, std::vector< const kernel::UrbanObject_ABC* >& intersectedBlocks );
    void ChangeShape( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& block, std::vector< const kernel::UrbanObject_ABC* >& intersectedBlocks );
    kernel::UrbanObject_ABC* Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent );
    void DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects );
    void DeleteBlock( const kernel::UrbanObject_ABC& urbanObject );
    bool TakeLinkErrors();
    void CreateGeostoreManager( const tools::ExerciseConfig& config );
    bool ManageIdConflicts();
    //@}

    //! @name Accessors
    //@{
    const kernel::UrbanObject_ABC* FindBlock( const geometry::Point2f& center ) const;
    void GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< const kernel::UrbanObject_ABC* >& result ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadCity( xml::xistream& xis );
    void ReadDistrict( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadBlock( xml::xistream& xis, kernel::UrbanObject_ABC* parent );
    void ReadUrbanObject( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, kernel::UrbanObject_ABC& object );
    template< typename T, typename U >
    void UpdateCapacity( xml::xistream& xis, kernel::UrbanObject_ABC& object );
    void CreateQuadTree( float width, float height );
    void InsertIntoQuadTree( const kernel::UrbanObject_ABC& urbanObject );
    void CleanLinks();
    void CheckIdConflict( kernel::UrbanObject_ABC& urban );
    void ReplaceLinkId( kernel::UrbanObject_ABC& urban );
    void ChangeUrbanObjectId( kernel::UrbanObject_ABC& urban, unsigned long newId );
    //@}

private:
    //! @name Types
    //@{
    struct UrbanIdReplacement
    {
        UrbanIdReplacement(): pUrban_( 0 ), newId_( 0 ) {}
        kernel::UrbanObject_ABC* pUrban_;
        unsigned long newId_;
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                         controllers_;
    const StaticModel&                           staticModel_;
    tools::IdManager&                            idManager_;
    const tools::Resolver< kernel::Object_ABC >& objects_;
    std::unique_ptr< gui::UrbanDisplayOptions >  urbanDisplayOptions_;
    std::unique_ptr< UrbanFactory_ABC >          factory_;
    std::unique_ptr< geostore::Geostore >        geostore_;
    std::auto_ptr< T_QuadTree >                  quadTree_;
    std::unique_ptr< UrbanMenuManager >          menuManager_;
    float                                        precision_;
    float                                        maxElementSize_;
    bool                                         cleanedLinks_;
    std::map< unsigned long, UrbanIdReplacement > urbanConflictIds_;
    //@}
};

#endif // __preparation_UrbanModel_h_
