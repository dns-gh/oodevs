// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodAttribute_h_
#define __FloodAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "propagation/ElevationGetter_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/shared_ptr.hpp>
#include <boost/serialization/export.hpp>

namespace propagation
{
    class FloodModel_ABC;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodAttribute : public ObjectAttribute_ABC
                     , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodAttribute();
             FloodAttribute( xml::xistream& xis, const TER_Localisation& objectLocation );
             FloodAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& objectLocation );
    virtual ~FloodAttribute();
    //@}

    //! @name Copy/Assignment
    //@{
    FloodAttribute& operator=( const FloodAttribute& ); //!< Assignment operator
    bool Update( const FloodAttribute& rhs );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network updates
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Operations
    //@{
    virtual void GenerateFlood( const propagation::FloodModel_ABC& model );
    virtual const TER_Localisation& GetLocalisation() const;
    void Apply( const std::function< void( const TER_Polygon&, bool ) >& func ) const;
    const std::vector< geometry::Polygon2f >& GetDeepAreas() const;
    const std::vector< geometry::Polygon2f >& GetLowAreas() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodAttribute( const FloodAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    int depth_;
    int refDist_;
    MT_Vector2D floodCenter_;
    TER_Localisation location_;
    std::vector< geometry::Polygon2f > deepAreas_;
    std::vector< geometry::Polygon2f > lowAreas_;
    std::vector< TER_Polygon > deepAreasLocalisation_;
    std::vector< TER_Polygon > lowAreasLocalisation_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FloodAttribute )

#endif // __FloodAttribute_h_
