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

namespace sword
{
    class MissionParameter_Value;
    enum ObjectMagicActionAck_ErrorCode;
}

class MIL_Army_ABC;
class MIL_ObjectType_ABC;
class MIL_Color;

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
             Object( xml::xistream& xis, const MIL_ObjectType_ABC& type, MIL_Army_ABC* army, const TER_Localisation* pLocation );
             Object( const MIL_ObjectType_ABC& type, MIL_Army_ABC* army, const TER_Localisation* pLocation,
                     unsigned int externalIdentifier, const std::string& name = std::string(), unsigned int forcedId = 0u );
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
    virtual void SendCreation() const;
    virtual void SendDestruction() const;
    virtual void SendFullState() const;
    //@}

    //! @name
    //@{
    const std::string& GetName() const;
    virtual void SetName( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned int externalIdentifier_;
    std::auto_ptr< MIL_Color > pColor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( Object )

#endif
