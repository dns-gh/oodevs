// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

#include "MIL_Object.h"
#include "CapacityContainer_ABC.h"
#include <tools/Extendable.h>
#include <vector>

namespace sword
{
    class MissionParameter_Value;
    enum ObjectMagicActionAck_ErrorCode;
}

class ObjectAttribute_ABC;
class MIL_ObjectType_ABC;
class MIL_Army_ABC;
class MIL_ObjectBuilder_ABC;
class DetectionCapacity;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: JCR 2008-04-18
// =============================================================================
class Object : public MIL_Object
{
public:
    //! @name Constructors/Destructor
    //@{
             Object( xml::xistream& xis, const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, bool reserved = true );
             Object( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army, const TER_Localisation* pLocation, const std::string& name = std::string(), bool reserved = true );
             Object();
    virtual ~Object();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Construction
    //@{
    void Update( unsigned int time );
    //@}

    //! @name Interaction
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent ) const;
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside( MIL_Agent_ABC& agent );

    //@}

    //! @name Network
    //@{
    virtual void UpdateState();

    sword::ObjectMagicActionAck_ErrorCode OnUpdate( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& attributes );
    sword::ObjectMagicActionAck_ErrorCode OnRequest( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& params );

    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    //@}

    //! @name
    //@{
    const std::string& GetName() const;
    virtual unsigned int  GetMaterial() const{ return std::numeric_limits< unsigned int >::max(); };   // $$$$ _TODO_ SLG 2010-06-24: mettre en place un système de material pour les objets
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Object( const Object& );            //!< Copy constructor
    Object& operator=( const Object& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    enum E_AttributeUpdate
    {
        eAttrUpdate_Localisation              = 0x10,
        eAttrUpdate_All                       = 0xFF
    };
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::auto_ptr< Object > pChildObject_;
    //@}

    //! @name Network
    //@{
    mutable unsigned char xAttrToUpdate_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( Object )

#endif
