// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanModel_h_
#define __MIL_UrbanModel_h_

#include <tools/Resolver.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/CoordinateConverter_ABC.h>
#include <spatialcontainer/SpatialContainer.h>

class MT_Vector2D;

// =============================================================================
/** @class  MIL_UrbanModel
    @brief  MIL_UrbanModel
*/
// Created: JSR 2012-07-26
// =============================================================================
class MIL_UrbanModel : public tools::Resolver< urban::TerrainObject_ABC >
                     , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct QuadTreeTraits;
    typedef spatialcontainer::SpatialContainer< const urban::TerrainObject_ABC*, QuadTreeTraits, float > T_QuadTree;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanModel();
    virtual ~MIL_UrbanModel();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& directoryPath );

    void GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< const urban::TerrainObject_ABC* >& result ) const;
    void GetListWithinSegment( const MT_Vector2D& start, const MT_Vector2D& end, std::vector< const urban::TerrainObject_ABC* >& result ) const;
    const urban::TerrainObject_ABC* FindBlock( const MT_Vector2D& point ) const;
    std::vector< const urban::TerrainObject_ABC* > GetCities() const;
    float GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    const urban::TerrainObject_ABC* GetTerrainObject( unsigned int id ) const;

    void Accept( urban::ObjectVisitor_ABC& visitor );
    //@}

private:
    //! @name Helpers
    //@{
    void Purge();
    void Load( xml::xistream& xis );
    void ReadCity( xml::xistream& xis );
    void ReadItem( xml::xistream& xis, urban::TerrainObject_ABC& parent );
    void CreateQuadTree( const geometry::Rectangle2f& rect );
    void RetrieveVersion( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string modelVersion_; // utile???
    std::auto_ptr< urban::CoordinateConverter_ABC > converter_;
    std::auto_ptr< T_QuadTree > quadTree_;
    float precision_;
    float maxElementSize_;
    //@}
};

#endif // __MIL_UrbanModel_h_
