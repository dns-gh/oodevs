// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ObjectFactory_h_
#define __MIL_ObjectFactory_h_

namespace Common
{
    enum ObstacleType_DemolitionTargetType;
}

namespace MsgsClientToSim
{
    class MsgMagicActionCreateObject;
}

namespace MsgsSimToClient
{
    enum MsgObjectMagicActionAck_ErrorCode;
}

class MIL_Army_ABC;
class MIL_Object_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectLoader;
class MIL_ObjectManager;
class MIL_ObjectType_ABC;
class TER_Localisation;

// =============================================================================
/** @class  MIL_ObjectFactory
    @brief  MIL_ObjectFactory
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectFactory
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ObjectFactory( MIL_ObjectManager& manager );
    virtual ~MIL_ObjectFactory();
    //@}    

    //! @name Methods
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    MIL_Object_ABC&                                     BuildObject( xml::xistream& xis, MIL_Army_ABC& army );
    MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode  BuildObject( const MsgsClientToSim::MsgMagicActionCreateObject& asn, MIL_Army_ABC& army );
    MIL_Object_ABC*                                     BuildObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, Common::ObstacleType_DemolitionTargetType obstacleType );
	MIL_Object_ABC*                                     BuildObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army );
    //@}

    //! @name Accessors
    //@{
    static const MIL_ObjectType_ABC& FindType( const std::string& type );    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectFactory( const MIL_ObjectFactory& );            //!< Copy constructor
    MIL_ObjectFactory& operator=( const MIL_ObjectFactory& ); //!< Assignment operator
    //@}

private:
    //! @name 
    //@{
    MIL_ObjectManager& manager_;
    //@}
};

#endif