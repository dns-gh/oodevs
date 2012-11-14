// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_UrbanObject_h_
#define __kernel_UrbanObject_h_

#include "UrbanObject_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "HumanDefs.h"
#include "StructuralStateAttribute_ABC.h"
#include "EntityImplementation.h"
#include "Extension_ABC.h"
#include "Displayable_ABC.h"

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
    class Controllers;
    class UrbanDisplayOptions;
    class AccommodationTypes;
    class ObjectType;
    class PropertiesDictionary;

// =============================================================================
/** @class  UrbanObject
    @brief  Urban object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject : public EntityImplementation< kernel::UrbanObject_ABC >
                  , public Extension_ABC
                  , public Displayable_ABC
                  , public kernel::Updatable_ABC< sword::UrbanUpdate >
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< UrbanDisplayOptions >
                  , public tools::ElementObserver_ABC< StructuralStateAttribute_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( Controllers& controllers, const std::string& name, unsigned int id,
                          const ObjectType& type, const AccommodationTypes& accommodations,
                          UrbanDisplayOptions& options);
             UrbanObject( xml::xistream& xis, Controllers& controllers, const ObjectType& type,
                          const AccommodationTypes& accommodations, UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::ObjectType& GetType() const { return type_; }
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UrbanUpdate& msg );
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Display( kernel::Displayer_ABC& ) const {}
    void UpdateHumans( const std::string& inhabitant, const sword::PopulationUpdate_BlockOccupation& occupation );
    float GetLivingSpace( bool forceUpdate = false ) const;
    float GetLivingSpace( unsigned int floorNumber, unsigned int occupation ) const;
    double GetNominalCapacity() const;
    double GetNominalCapacity( const std::string& motivation ) const;
    const kernel::AccommodationTypes& GetAccommodations() const;
    const T_HumansStrMap& GetHumansMap() const { return humans_; }
    virtual void UpdateColor();
    virtual void NotifyUpdated( const UrbanDisplayOptions& );
    virtual void NotifyUpdated( const StructuralStateAttribute_ABC& );
    void CreateDictionary( bool readOnly );
    virtual void ApplyTemplate( const UrbanTemplateTypePtr& urbanTemplate );
    virtual void UpdateTemplate( const ObjectTypes& objectTypes );
    virtual bool IsUpdatingTemplate() const; // loic boom
    //@}

private:
    //! @name Helpers
    //@{
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
    UrbanTemplateTypePtr templateType_;
    const kernel::AccommodationTypes& accommodations_;
    mutable float livingSpace_;
    mutable double nominalCapacity_;
    bool updatingTemplate_;
    bool fillingTemplate_;
    //@}

protected:
    //! @name Member data
    //@{
    Controllers& controllers_;
    UrbanDisplayOptions& options_;
    //@}
};

} //! using namespace kernel

#endif // __kernel_UrbanObject_h_
