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

namespace xml
{
    class xistream;
}

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    class MissionParameters;
    enum ObjectMagicActionAck_ErrorCode;
}

namespace urban
{
    class TerrainObject_ABC;
}

class MIL_Army_ABC;
class MIL_Object_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectLoader;
class MIL_ObjectType_ABC;
class MIL_ObjectFilter;
class TER_Localisation;

// =============================================================================
/** @class  MIL_ObjectFactory
    @brief  MIL Object factory
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_ObjectFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectFactory();
    virtual ~MIL_ObjectFactory();
    //@}

    //! @name Methods
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    MIL_Object_ABC* BuildObject( xml::xistream& xis, MIL_Army_ABC* army );
    MIL_Object_ABC* BuildObject( const sword::MissionParameters& asn, MIL_Army_ABC* army, sword::ObjectMagicActionAck_ErrorCode& value );
    MIL_Object_ABC* BuildObject( const std::string& name, const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation,
                                 sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier, unsigned int forcedId = 0u );
    MIL_Object_ABC* BuildObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army );
    MIL_Object_ABC* BuildUrbanObject( const urban::TerrainObject_ABC& object );
    //@}

    //! @name Accessors
    //@{
    static const MIL_ObjectType_ABC& FindType( const std::string& type );
    static const void FindDangerousIDs( std::vector< unsigned int >& dangerousIDs, const MIL_ObjectFilter& filter );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectFactory( const MIL_ObjectFactory& );            //!< Copy constructor
    MIL_ObjectFactory& operator=( const MIL_ObjectFactory& ); //!< Assignment operator
    //@}
};

#endif // __MIL_ObjectFactory_h_
