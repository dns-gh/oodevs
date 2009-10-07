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
#include "game_asn/generated/Object.h"
#include <boost/shared_ptr.hpp>

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
    MIL_Object_ABC* CreateObject( xml::xistream& xis, MIL_Army_ABC& army ) const;
    MIL_Object_ABC* CreateObject( const ASN1T_MagicActionCreateObject& asn, MIL_Army_ABC& army, ASN1T_EnumObjectErrorCode& value ) const;
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& location, bool reserved, const std::vector< double >& parameters ) const;
    MIL_Object_ABC* CreateObject( const MIL_ObjectBuilder_ABC& builder, MIL_Army_ABC& army ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_ObjectType_ABC& GetType( const std::string& type ) const;
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
    ASN1T_EnumObjectErrorCode InitializeLocation( Object& object, const ASN1T_Location& asn ) const;
    //@}

private:
    //! @name Data members
    //@{
    std::auto_ptr< CapacityFactory > factory_;
    std::auto_ptr< AttributeFactory > attributes_;
    T_Prototypes prototypes_;
    //@}
};

#endif
