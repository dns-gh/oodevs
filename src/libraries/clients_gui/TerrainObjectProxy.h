// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainObjectProxy_h_
#define __TerrainObjectProxy_h_

#include "HumanDefs.h"
#include "clients_kernel/Creatable.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
    class Displayer_ABC;
    class ObjectType;
    class PropertiesDictionary;
}

namespace sword
{
    class PopulationUpdate_BlockOccupation;
    class UrbanUpdate;
}

namespace gui
{
    class UrbanDisplayOptions;

// =============================================================================
/** @class  TerrainObjectProxy
    @brief  Terrain object proxy
*/
// Created: SLG 2009-02-10
// =============================================================================
class TerrainObjectProxy : public kernel::Extension_ABC
                         , public kernel::EntityImplementation< kernel::Object_ABC >
                         , public kernel::Updatable_ABC< sword::UrbanUpdate >
                         , public kernel::Creatable< TerrainObjectProxy >
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< UrbanDisplayOptions >
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TerrainObjectProxy( kernel::Controllers& controllers, const std::string& name, unsigned int id
                 , const kernel::ObjectType& type, UrbanDisplayOptions& options, const kernel::AccommodationTypes& accommodations );
    virtual ~TerrainObjectProxy();
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
    void NotifyUpdated( const UrbanDisplayOptions& );
    float GetLivingSpace() const;
    double GetNominalCapacity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainObjectProxy( const TerrainObjectProxy& );           //!< Copy constructor
    TerrainObjectProxy& operator=( const TerrainObjectProxy& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    void UpdateDensity();
    unsigned int GetHumans() const;
    void UpdateColor();
    double GetNominalCapacity( const std::string& motivation ) const;
    //@}

    //! @name Types
    //@{
    typedef T_HumansStrMap T_Humans;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    float density_;
    T_Humans humans_;
    T_BlockOccupation motivations_;
    const kernel::ObjectType& type_;
    const kernel::AccommodationTypes& accommodations_;
    mutable float livingSpace_;
    mutable double nominalCapacity_;
    UrbanDisplayOptions& options_;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
