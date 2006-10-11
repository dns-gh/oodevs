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

#include "Resolver.h"
#include "Resolver2.h"
#include "IDManager.h"

namespace xml { class xistream; };

namespace kernel
{
    class ObjectType;
    class DotationType;
    class EquipmentType;
    class NBCAgent;
    class BreakdownType;

// =============================================================================
/** @class  ObjectTypes
    @brief  ObjectTypes
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectTypes
    : public Resolver< ObjectType >
    , public Resolver2< DotationType > // $$$$ AGE 2006-04-05: pas du tout des objets...
    , public Resolver< EquipmentType >
    , public Resolver< NBCAgent >
    , public Resolver< BreakdownType >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectTypes();
    virtual ~ObjectTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectTypes( const ObjectTypes& );            //!< Copy constructor
    ObjectTypes& operator=( const ObjectTypes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< int         , unsigned int >  T_ObjectIDs;
    typedef std::map< unsigned int, IDManager* >    T_Managers;
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

}

#endif // __ObjectTypes_h_
