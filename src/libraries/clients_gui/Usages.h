// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_Usages_h_
#define __gui_Usages_h_

#include "clients_kernel/Usages_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace gui
{
// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: ABR 2012-05-22
// =============================================================================
class Usages : public kernel::Usages_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages( PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual bool IsDefault() const;
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual void Remove( const std::string& usage );
    virtual unsigned int Find( const std::string& usage ) const;
    virtual const kernel::T_Usages& GetUsages() const { return usages_; }
    virtual const T_Occupations& GetOccupations() const { return occupations_; }
    virtual void ClearAll();
    virtual void UpdateMotivations( float livingSpace );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateDefault();
    //@}

protected:
    //! @name Member Data
    //@{
    kernel::Entity_ABC&               owner_;
    PropertiesDictionary&             dictionary_;
    const kernel::AccommodationTypes& accommodationTypes_;
    float                             livingSpace_;
    kernel::T_Usages                  usages_;
    T_Occupations                     occupations_;
    //@}

public:
    //! @name Static member data
    //@{
    static const std::string  defaultStr_;
    //@}
};

} //! namespace kernel

#endif // __gui_Usages_h_
