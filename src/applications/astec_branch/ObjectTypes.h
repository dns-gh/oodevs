// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectTypes_h_
#define __ObjectTypes_h_

#include "ASN_Types.h"
#include "Resolver.h"
#include "IDManager.h"

class ObjectType;
class DotationType;
namespace xml { class xistream; };

// =============================================================================
/** @class  ObjectTypes
    @brief  ObjectTypes
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectTypes
    : public Resolver< ObjectType >
    , public Resolver< DotationType >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectTypes( const std::string& scipioXml );
    virtual ~ObjectTypes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectTypes( const ObjectTypes& );            //!< Copy constructor
    ObjectTypes& operator=( const ObjectTypes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< ASN1T_EnumObjectType, unsigned int >  T_ObjectIDs;
    typedef std::map< unsigned int, IDManager* >            T_Managers;
    struct Reader  {
        void Read( xml::xistream& xis, ObjectTypes& objects );
    };
    void ReadDotations( const std::string& dotations );
    void ReadDotation( xml::xistream& xis );
    void ReadCategory( xml::xistream& xis, const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    T_ObjectIDs objectIds_;
    T_Managers  managers_;
    //@}
};

#endif // __ObjectTypes_h_
