// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Usages_ABC_h_
#define __Usages_ABC_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"
#include "HumanDefs.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class Entity_ABC;

// =============================================================================
/** @class  Usages_ABC
    @brief  Usages declaration
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages_ABC : public Extension_ABC
                 , public Displayable_ABC
                 , public Serializable_ABC
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages_ABC() {}
    virtual ~Usages_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< unsigned int, unsigned int > > T_Occupations;
    typedef T_Occupations::iterator                                         IT_Occupations;
    typedef T_Occupations::const_iterator                                  CIT_Occupations;
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion ) = 0;
    virtual void Remove( const std::string& usage ) = 0;
    virtual unsigned int Find( const std::string& usage ) const = 0;
    virtual const T_Occupations& GetOccupations() const = 0;
    virtual const T_Usages& GetUsages() const = 0;
    virtual void ClearAll() = 0;
    virtual void UpdateMotivations( float livingSpace ) = 0;
    //@}
};

}

#endif // __Usages_ABC_h_
