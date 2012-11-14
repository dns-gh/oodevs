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

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class Entity_ABC;
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
             Usages( PropertiesDictionary& dictionary, const AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~Usages();
    //@}

    //! @name Operations
    //@{
    virtual bool IsDefault() const;
    virtual void Add( const std::string& usage, unsigned int proportion );
    virtual void Remove( const std::string& usage );
    virtual unsigned int Find( const std::string& usage ) const;
    virtual const T_Usages& GetUsages() const { return usages_; }
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
    Controller&               controller_;
    Entity_ABC&               owner_;
    PropertiesDictionary&     dictionary_;
    const AccommodationTypes& accommodationTypes_;
    float                     livingSpace_;
    T_Usages                  usages_;
    T_Occupations             occupations_;
    //@}

public:
    //! @name Static member data
    //@{
    static const std::string  defaultStr_;
    //@}
};

} //! namespace kernel

#endif // __kernel_Usages_h_
