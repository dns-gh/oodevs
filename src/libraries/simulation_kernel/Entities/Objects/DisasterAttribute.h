// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DisasterAttribute_h
#define DisasterAttribute_h

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include "MIL.h"
#include <tools/Path.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <memory>

namespace xml
{
    class xistream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class PropagationManager;
class Extractor_ABC;

// =============================================================================
/** @class  DisasterAttribute
    @brief  Disaster attribute
*/
// Created: LGY 2012-10-05
// =============================================================================
class DisasterAttribute : public ObjectAttribute_ABC
                        , public UpdatableAttribute_ABC
{
public:
    //! @name Types
    //@{
    typedef DEC_Knowledge_ObjectAttributeProxyPassThrough< DisasterAttribute > T_KnowledgeProxyType;
    typedef boost::shared_ptr< Extractor_ABC > T_Extractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DisasterAttribute();
    explicit DisasterAttribute( xml::xistream& xis );
    explicit DisasterAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~DisasterAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;

    void SendFullState( sword::ObjectAttributes& asn ) const;
    bool SendUpdate( sword::ObjectAttributes& asn ) const;
    void UpdateLocalisation( MIL_Object_ABC& object, unsigned int time );
    bool Update( const DisasterAttribute& rhs );
    const std::vector< T_Extractor >& GetExtractors() const;
    float GetDose( const MT_Vector2D& position ) const;
    double ApplySpeedPolicy( const MIL_Entity_ABC& entity, double speed ) const;
    DisasterAttribute& operator=( const DisasterAttribute& ); //!< Assignment operator
    void OnUpdate( const sword::MissionParameter_Value& /*attribute*/ );
    //@}

private:
    //! @name Member data
    //@{
    tools::Path model_;
    std::string date_;
    std::auto_ptr< PropagationManager > pManager_;
    std::vector< T_Extractor > values_;
    std::vector< tools::Path > files_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DisasterAttribute )

#endif // DisasterAttribute_h
