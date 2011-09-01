// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
}

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class ObstacleAttribute : public ObjectAttribute_ABC
                        , public UpdatableAttribute_ABC
{
public:
    typedef DEC_Knowledge_ObjectAttributeProxyPassThrough< ObstacleAttribute > T_KnowledgeProxyType;

public:
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute();
    explicit ObstacleAttribute( bool reserved );
    explicit ObstacleAttribute( xml::xistream& xis );
    explicit ObstacleAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~ObstacleAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void SetType( sword::ObstacleType_DemolitionTargetType obstacleType );
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual void SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name
    //@{
    int GetActivationTime() const;
    int GetEndActivity() const;
    bool IsActivable() const;
    bool IsDeactivable() const;
    bool IsActivated() const;
    void Activate();
    void Deactivate();
    //@}

    //! @name Operations
    //@{
    virtual void OnUpdate( const sword::MissionParameter_Value& attribute ); //<! Update from asn msg
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    bool Update( const ObstacleAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    sword::ObstacleType_DemolitionTargetType obstacle_;
    bool bActivated_;
    int activationTime_;
    int activityTime_;
    int endActivity_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ObstacleAttribute )

#endif // __ObstacleAttribute_h_
