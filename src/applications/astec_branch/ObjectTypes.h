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
class EquipmentType;
class NBCAgent;
class BreakdownType;

namespace xml { class xistream; };

// =============================================================================
/** @class  ObjectTypes
    @brief  ObjectTypes
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectTypes
    : public Resolver< ObjectType >
    , public Resolver< DotationType > // $$$$ AGE 2006-04-05: pas du tout des objets...
    , public Resolver< EquipmentType >
    , public Resolver< NBCAgent >
    , public Resolver< BreakdownType >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectTypes( const std::string& scipioXml );
    virtual ~ObjectTypes();
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
    //@}

    //! @name Helpers
    //@{
    void ReadObjectTypes( xml::xistream& xis );
    void ReadDotations( const std::string& dotations );
    void ReadDotation( xml::xistream& xis );
    void ReadCategory( xml::xistream& xis, const std::string& name );
    void ReadEquipments( const std::string& equipments );
    void ReadEquipment( xml::xistream& xis );
    void ReadNBC( const std::string& nbc );
    void ReadNBCAgent( xml::xistream& xis );
    void ReadBreakdowns( const std::string& breakdowns );
    void ReadBreakdown( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_ObjectIDs objectIds_;
    T_Managers  managers_;
    //@}
};

#endif // __ObjectTypes_h_
