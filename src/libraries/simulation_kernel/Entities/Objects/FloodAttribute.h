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
#include "flood/ElevationGetter_ABC.h"
#include "knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/serialization/export.hpp>

namespace flood
{
    class FloodModel;
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
                     , public flood::ElevationGetter_ABC
{
public:
    typedef DEC_Knowledge_ObjectAttributeProxyPassThrough< FloodAttribute > T_KnowledgeProxyType;

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

    //! @name From ElevationGetter_ABC
    //@{
    virtual short GetElevationAt( const geometry::Point2f& point ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network updates
    //@{
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute );
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Operations
    //@{
    void GenerateFlood( bool force = false );
    bool ReadFromODB() const;
    const TER_Localisation& GetLocalisation() const;
    const std::vector< geometry::Polygon2f* >& GetDeepAreas() const;
    const std::vector< geometry::Polygon2f* >& GetLowAreas() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodAttribute( const FloodAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< flood::FloodModel > floodModel_;
    bool readFromODB_;
    int depth_;
    int refDist_;
    TER_Localisation location_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FloodAttribute )

#endif // __FloodAttribute_h_
