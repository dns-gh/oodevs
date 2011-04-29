// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _Affinities_ABC_h
#define _Affinities_ABC_h

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "Types.h"
#include <map>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Affinities
    @brief  Affinities declaration
*/
// Created: LGY 2011-03-17
// =============================================================================
class Affinities : public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Affinities();
    explicit Affinities( xml::xistream& xis );
    virtual ~Affinities();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< unsigned long, EntityAffinity > T_Affinities;
    typedef T_Affinities::const_iterator            CIT_Affinities;
    //@}

    //! @name Operations
    //@{
    virtual void Clear() = 0;
    virtual void Add( unsigned long team, EntityAffinity value );
    virtual void Fill( std::map< unsigned long, EntityAffinity >& content );
    virtual bool HasAffinities() const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAffinity( xml::xistream& xis );
    //@}


protected:
    //! @name Member data
    //@{
    T_Affinities affinities_;
    //@}
};

#endif // _Affinities_ABC_h
