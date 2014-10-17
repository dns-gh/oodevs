// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Entity_ABC_h_
#define __MIL_Entity_ABC_h_

#include "MT_Tools/RoleContainer.h"
#include "Entities/Actions/PHY_Actor.h"
#pragma warning( push, 0 )
#include <boost/archive/binary_oarchive_impl.hpp>
#pragma warning( pop ) // $$$$ LDC Mandatory in order for is_abstract stuff below to compile
#include <boost/serialization/export.hpp>  

namespace xml
{
    class xistream;
    class xostream;
}

class MIL_Army_ABC;
class MIL_EntityType;

// =============================================================================
/** @class  MIL_Entity_ABC
    @brief  MIL Entity definition
*/
// Created: RDS 2008-04-25
// =============================================================================
class MIL_Entity_ABC : public tools::RoleContainer
                     , public PHY_Actor // $$$$ LDC: That should be a role.
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Entity_ABC( const std::string& name, unsigned int id );
             MIL_Entity_ABC( xml::xistream& xis, unsigned int id );
    virtual ~MIL_Entity_ABC();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const std::string& GetName() const;
    void SetName( const std::string& name );
    virtual MIL_Army_ABC& GetArmy() const = 0;
    virtual bool CanEmitReports() const = 0;
    virtual const MIL_EntityType& GetType() const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name ODB
    //@{
    void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name data Members
    //@{
    std::string strName_;
    const unsigned int id_;
    //@}
};

// BOOST_SERIALIZATION_ASSUME_ABSTRACT should be used for this
// but it seems to be buggy : inherits boost::is_abstract<T> instead of boost::true_type
namespace boost {
namespace serialization {
    template<>
    struct is_abstract<const MIL_Entity_ABC> : boost::true_type {};
} // namespace serialization
}

BOOST_CLASS_EXPORT_KEY( MIL_Entity_ABC )

#endif // __MIL_Entity_ABC_h_
