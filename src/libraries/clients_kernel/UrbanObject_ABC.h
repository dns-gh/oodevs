// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObject_ABC_h_
#define __UrbanObject_ABC_h_

#include "HumanDefs.h"
#include "Creatable.h"
#include "EntityImplementation.h"
#include "Extension_ABC.h"
#include "Object_ABC.h"
#include <boost/noncopyable.hpp>
namespace sword
{
    class PopulationUpdate_BlockOccupation;
    class UrbanUpdate;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class Displayer_ABC;
    class ObjectType;
    class PropertiesDictionary;

// =============================================================================
/** @class  UrbanObject_ABC
    @brief  Urban Object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject_ABC : public kernel::Extension_ABC
                      , public kernel::EntityImplementation< kernel::Object_ABC >
                      , public kernel::Updatable_ABC< sword::UrbanUpdate >
                      , public kernel::Creatable< UrbanObject_ABC >
                      , private boost::noncopyable
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject_ABC( kernel::Controller& controller, const std::string& name, unsigned int id,
                              const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations );
             UrbanObject_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::ObjectType& type,
                              const kernel::AccommodationTypes& accommodations );
    virtual ~UrbanObject_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::ObjectType& GetType() const { return type_; }
    virtual QString GetTypeName() const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanUpdate& msg );
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Display( kernel::Displayer_ABC& ) const {}
    void UpdateHumans( const std::string& inhabitant, const sword::PopulationUpdate_BlockOccupation& occupation );
    float GetLivingSpace() const;
    double GetNominalCapacity() const;
    double GetNominalCapacity( const std::string& motivation ) const;
    const kernel::AccommodationTypes& GetAccommodations() const;
    const T_HumansStrMap& GetHumansMap() const { return humans_; }
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void UpdateColor() = 0;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void UpdateDensity();
    unsigned int GetHumans() const;
    //@}

    //! @name Types
    //@{
    typedef T_HumansStrMap T_Humans;
    //@}

private:
    //! @name Member data
    //@{
    float density_;
    T_Humans humans_;
    T_BlockOccupation motivations_;
    const kernel::ObjectType& type_;
    const kernel::AccommodationTypes& accommodations_;
    mutable float livingSpace_;
    mutable double nominalCapacity_;
    //@}
};

}

#endif // __UrbanObject_ABC_h_
