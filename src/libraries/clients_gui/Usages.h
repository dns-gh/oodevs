// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_Usages_h
#define gui_Usages_h

#include "clients_kernel/Usages_ABC.h"
#include "HumanDefs.h"

namespace kernel
{
    class AccommodationTypes;
    class PropertiesDictionary;
}

namespace gui
{
// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages : public kernel::Usages_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Usages( kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual unsigned int Find( const std::string& usage ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< unsigned int, unsigned int > > T_Occupations;
    //@}

private:
    //! @name Member Data
    //@{
    kernel::PropertiesDictionary& dictionary_;
    const kernel::AccommodationTypes& accommodationTypes_;
    float livingSpace_;
    T_Usages usages_;
    T_Occupations occupations_;
    static const std::string defaultStr_;
    //@}
};

}

#endif // gui_Usages_h
