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

#include "MT_Tools/MT_RoleContainer.h"

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  MIL_Entity_ABC
    @brief  MIL_Entity_ABC
*/
// Created: RDS 2008-04-25
// =============================================================================
class MIL_Entity_ABC : public MT_RoleContainer
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Entity_ABC();
    explicit MIL_Entity_ABC( const std::string& name );
             MIL_Entity_ABC( const std::string& name, xml::xistream& xis );
    virtual ~MIL_Entity_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const uint );
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

    //! @name data Members
    //@{
    std::string strName_;
    //@}

};

#include "MIL_Entity_ABC.inl"

#endif // __MIL_Entity_ABC_h_
