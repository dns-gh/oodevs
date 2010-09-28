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
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

namespace Common
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
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute();
    explicit ObstacleAttribute( bool reserved );
    explicit ObstacleAttribute( xml::xistream& xis );
    explicit ObstacleAttribute( const Common::MsgMissionParameter_Value& attributes );
    virtual ~ObstacleAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& file, const unsigned int );
    //@}


    //! @name Operations
    //@{
    void SetType( Common::ObstacleType_DemolitionTargetType obstacleType );
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( Common::ObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name
    //@{
    int GetActivationTime() const;
    bool IsActivable() const;
    bool IsActivated() const;
    void Activate();
    //@}

    //! @name Operations
    //@{
    virtual void OnUpdate( const Common::MsgMissionParameter_Value& attribute ); //<! Update from asn msg
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Common::ObstacleType_DemolitionTargetType obstacle_;
    bool bActivated_;
    int activationTime_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ObstacleAttribute )

#endif // __ObstacleAttribute_h_
