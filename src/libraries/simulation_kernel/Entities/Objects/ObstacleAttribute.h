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

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class ObstacleAttribute 
    : public ObjectAttribute_ABC
    , public UpdatableAttribute_ABC
{ 
public:
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute();
    explicit ObstacleAttribute( bool reserved );
    explicit ObstacleAttribute( xml::xistream& xis );
    explicit ObstacleAttribute( const ASN1T_ObjectAttributes& asn );
    virtual ~ObstacleAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& file, const uint );
    //@}


    //! @name 
    //@{
    void SetType( ASN1T_EnumDemolitionTargetType obstacleType );
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name 
    //@{
    bool IsActivable() const;
    bool IsActivated() const;
    void Activate();
    //@}

    //! @name Operations
    //@{
    virtual void OnUpdate( const ASN1T_ObjectAttributes& asn ); //<! Update from ASN msg
    ObstacleAttribute& operator=( const ObstacleAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Type
    //@{
    typedef EnumDemolitionTargetType::Root E_DemolitionTargetType;
    //@}

private:
    //! @name Member data
    //@{
    E_DemolitionTargetType  obstacle_;
    bool                    bActivated_;
    //@}
};

#endif // __ObstacleAttribute_h_
