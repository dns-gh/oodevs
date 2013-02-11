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

namespace xml
{
    class xistream;
    class xostream;
}

class MIL_Army_ABC;

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
    explicit MIL_Entity_ABC( const std::string& name );
    explicit MIL_Entity_ABC( xml::xistream& xis );
    virtual ~MIL_Entity_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    virtual MIL_Army_ABC& GetArmy() const = 0;
    virtual bool CanEmitReports() const = 0;
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
    //! @name Copy/Assignment
    //@{
    MIL_Entity_ABC( const MIL_Entity_ABC& );            //!< Copy constructor
    MIL_Entity_ABC& operator=( const MIL_Entity_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name data Members
    //@{
    const std::string strName_;
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

// -----------------------------------------------------------------------------
// Name: MIL_Entity_ABC::serialize
// Created: RDS 2008-05-09
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_Entity_ABC::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< tools::RoleContainer >( *this )
        & const_cast< std::string& >( strName_ ) ;
}

#endif // __MIL_Entity_ABC_h_
