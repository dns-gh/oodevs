// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_Usages_h_
#define __kernel_Usages_h_

#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/HumanDefs.h"

namespace kernel
{
    class AccommodationTypes;
    class PropertiesDictionary;

// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: ABR 2012-05-22
// =============================================================================
class Usages : public Usages_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages( PropertiesDictionary& dictionary, const AccommodationTypes& accommodationTypes, float livingSpace );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual unsigned int Find( const std::string& usage ) const;
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< unsigned int, unsigned int > > T_Occupations;
    //@}

protected:
    //! @name Member Data
    //@{
    PropertiesDictionary&     dictionary_;
    const AccommodationTypes& accommodationTypes_;
    float                     livingSpace_;
    T_Usages                  usages_;
    T_Occupations             occupations_;
    static const std::string  defaultStr_;
    //@}
};

} //! namespace kernel

#endif // __kernel_Usages_h_
