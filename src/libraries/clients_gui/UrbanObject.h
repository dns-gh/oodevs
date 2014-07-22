// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_URBANOBJECT_H__
#define CLIENTS_GUI_URBANOBJECT_H__

#include "EntityImplementation.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/HumanDefs.h"
#include "clients_kernel/StructuralStateAttribute_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include <tools/ElementObserver_ABC.h>

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
    class AccommodationTypes;
    class ObjectType;
}

namespace gui
{
    class UrbanDisplayOptions;
}

namespace gui
{
// =============================================================================
/** @class  UrbanObject
    @brief  Urban object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject : public gui::EntityImplementation< kernel::UrbanObject_ABC >
                  , public kernel::Extension_ABC
                  , public kernel::Displayable_ABC
                  , public kernel::Updatable_ABC< sword::UrbanUpdate >
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< UrbanDisplayOptions >
                  , public tools::ElementObserver_ABC< kernel::StructuralStateAttribute_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject( kernel::Controllers& controllers, const std::string& name, unsigned int id,
                          const kernel::ObjectType& type, const kernel::AccommodationTypes& accommodations,
                          UrbanDisplayOptions& options);
             UrbanObject( xml::xistream& xis, kernel::Controllers& controllers, const kernel::ObjectType& type,
                          const kernel::AccommodationTypes& accommodations, UrbanDisplayOptions& options );
    virtual ~UrbanObject();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::ObjectType& GetType() const { return type_; }
    //@}

    //! @name Types
    //@{
    typedef kernel::T_HumansStrMap T_Humans;
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
    const T_Humans& GetHumansMap() const { return humans_; }
    virtual void UpdateColor();
    virtual void NotifyUpdated( const UrbanDisplayOptions& );
    virtual void NotifyUpdated( const kernel::StructuralStateAttribute_ABC& );
    void CreateDictionary( bool readOnly );
    virtual void ApplyTemplate( const UrbanTemplateTypePtr& urbanTemplate );
    virtual void UpdateTemplate( const kernel::ObjectTypes& objectTypes );
    virtual bool IsUpdatingTemplate() const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateDensity();
    unsigned int GetHumans() const;
    //@}

private:
    //! @name Member data
    //@{
    float density_;
    T_Humans humans_;
    kernel::T_BlockOccupation motivations_;
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
    kernel::Controllers& controllers_;
    UrbanDisplayOptions& options_;
    //@}
};

} //! using namespace kernel

#endif // CLIENTS_GUI_URBANOBJECT_H__
