// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BridgingCapacity_h_
#define __BridgingCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include <spatialcontainer/TerrainData.h>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class TER_DynamicData;

// =============================================================================
/** @class  BridgingCapacity
    @brief  BridgingCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BridgingCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BridgingCapacity();
    explicit BridgingCapacity( xml::xistream& xis );
    virtual ~BridgingCapacity();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );
    virtual void CreatePathData();
    //@}

    //! @name Accessors
    //@{
    bool IsBridgeType() const;
    bool IsPathData() const;
    //@}

private:
    //! @name Copy
    //@{
    BridgingCapacity( const BridgingCapacity& from );
    //@}

    //! @name Helpers
    //@{
    void CreateBridgeGeometry( const T_PointVector& points );
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    T_PointVector bridge_;
    boost::shared_ptr< TER_DynamicData > data_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BridgingCapacity )

#endif
