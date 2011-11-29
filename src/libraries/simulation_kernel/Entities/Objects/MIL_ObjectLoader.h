// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ObjectLoader_h_
#define __MIL_ObjectLoader_h_

#include "MIL_ObjectTypeHandler_ABC.h"
#include <boost/shared_ptr.hpp>

namespace sword
{
    class Location;
    class MissionParameters;
    enum ObjectMagicActionAck_ErrorCode;
}

namespace urban
{
    class TerrainObject_ABC;
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

// =============================================================================
/** @class  MIL_ObjectLoader
    @brief  MIL_ObjectLoader
*/
// Created: JCR 2008-05-23
// =============================================================================
class MIL_ObjectLoader : public MIL_ObjectTypeHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectLoader();
    virtual ~MIL_ObjectLoader();
    //@}

    //! @name Operations
    //@{
    void Initialize( xml::xistream& xis );
    MIL_Object_ABC* CreateObject( xml::xistream& xis, MIL_Army_ABC* army ) const;
    MIL_Object_ABC* CreateObject( const sword::MissionParameters& asn, MIL_Army_ABC* army, sword::ObjectMagicActionAck_ErrorCode& value ) const;
    MIL_Object_ABC* CreateObject( const std::string& name, const std::string& type, MIL_Army_ABC* army, const TER_Localisation& location,
                                  bool reserved, unsigned int externalIdentifier, unsigned int forcedId ) const;
    MIL_Object_ABC* CreateObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC* army ) const;
    MIL_Object_ABC* CreateUrbanObject( const urban::TerrainObject_ABC& object ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_ObjectType_ABC& GetType( const std::string& type ) const;
    const CapacityFactory& GetCapacityFactory() const;
    const void GetDangerousIDs( std::vector< unsigned int >& dangerousIDs, const MIL_ObjectFilter& filter ) const;
    //@}

    //! @name Singleton
    //@{
    static MIL_ObjectLoader& GetLoader();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectLoader( const MIL_ObjectLoader& );            //!< Copy constructor
    MIL_ObjectLoader& operator=( const MIL_ObjectLoader& ); //!< Assignment operator
    //@}

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
    sword::ObjectMagicActionAck_ErrorCode InitializeLocation( Object& object, const sword::Location& asn ) const;
    //@}

    template< class UnaryFunction >
    void ApplyOnPrototypes( UnaryFunction& functor ) const
    {
        for( CIT_Prototypes it = prototypes_.begin(); it != prototypes_.end(); ++it )
            functor( *it->second );
    }

private:
    //! @name Member data
    //@{
    std::auto_ptr< CapacityFactory > factory_;
    std::auto_ptr< AttributeFactory > attributes_;
    T_Prototypes prototypes_;
    //@}
};

#endif
