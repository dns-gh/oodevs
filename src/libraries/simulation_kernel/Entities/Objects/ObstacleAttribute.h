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
    explicit ObstacleAttribute( const Common::MsgObjectAttributes& asn );
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
    virtual void SendFullState( Common::MsgObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::MsgObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name 
    //@{
    bool IsActivable() const;
    bool IsActivated() const;
    void Activate();
    //@}

    //! @name Operations
    //@{
    virtual void OnUpdate( const Common::MsgObjectAttributes& asn ); //<! Update from asn msg
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Common::ObstacleType_DemolitionTargetType obstacle_;
    bool bActivated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ObstacleAttribute )

#endif // __ObstacleAttribute_h_
