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

#include "ObjectTypeResolver_ABC.h"
#include <boost/shared_ptr.hpp>

namespace sword
{
    class MissionParameters;
    enum ObjectMagicActionAck_ErrorCode;
}

namespace xml
{
    class xistream;
}

class CapacityFactory;
class AttributeFactory;
class ObjectPrototype;
class Object;
class TER_Localisation;
class MIL_Object_ABC;
class MIL_Army_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectFilter;
class MIL_UrbanObject_ABC;
class CapacityFactory_ABC;

// =============================================================================
/** @class  MIL_ObjectFactory
    @brief  MIL object factory
*/
// Created: JCR 2008-05-23
// =============================================================================
class MIL_ObjectFactory : public ObjectTypeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectFactory();
    virtual ~MIL_ObjectFactory();
    //@}

    //! @name Operations
    //@{
    void Initialize( xml::xistream& xis );
    MIL_Object_ABC* CreateObject( xml::xistream& xis, MIL_Army_ABC* army ) const;
    MIL_Object_ABC* CreateObject( const sword::MissionParameters& asn, MIL_Army_ABC* army ) const;
    MIL_Object_ABC* CreateObject( const std::string& name, const std::string& type, MIL_Army_ABC* army, const TER_Localisation& location,
                                  bool activated, unsigned int externalIdentifier, unsigned int forcedId = 0u, double density = 0. ) const;
    MIL_Object_ABC* CreateObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army ) const;
    MIL_UrbanObject_ABC* CreateUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent ) const;
    void Update( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object ) const;
    //@}

    //! @name Accessors
    //@{
    std::vector< unsigned int > GetDangerousObjects() const;
    double GetMaxAvoidanceDistance() const;
    //@}

    //! @name Resolvers
    //@{
    virtual const MIL_ObjectType_ABC& FindType( const std::string& type ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< ObjectPrototype > > T_Prototypes;
    typedef T_Prototypes::const_iterator                                CIT_Prototypes;
    //@}

    //! @name Helpers
    //@{
    void ReadObjectPrototype( xml::xistream& xis );
    void ReadCapacity( const std::string& capacity, xml::xistream& xis, ObjectPrototype& prototype );
    void ReadAttributes( const std::string& attribute, xml::xistream& xis, Object& object ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< CapacityFactory_ABC > factory_;
    std::unique_ptr< AttributeFactory > attributes_;
    T_Prototypes prototypes_;
    //@}
};

#endif // __MIL_ObjectFactory_h_
