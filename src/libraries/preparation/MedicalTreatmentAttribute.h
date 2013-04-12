// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentAttribute_h_
#define __MedicalTreatmentAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Displayer_ABC;
    class Entity_ABC;
    class MedicalTreatmentType;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalTreatmentAttribute : public kernel::MedicalTreatmentAttribute_ABC
                                , public kernel::Serializable_ABC
                                , public tools::Observer_ABC
                                , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
                                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute( const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes,
                                        gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& owner, bool bUrban = false, kernel::Controllers* controllers = 0 );
             MedicalTreatmentAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes,
                                        gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& owner, bool bUrban = false, kernel::Controllers* controllers = 0 );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& object );
    void Update( xml::xistream& xis );
    //@}

    //! @name Modifiers
    //@{
    void SetDoctors( unsigned n );
    void SetReferenceID( const std::string& id );
    void UpdateTreatmentCapacity( const std::string& type, unsigned beds );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentAttribute( const MedicalTreatmentAttribute& );            //!< Copy constructor
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsSet() const;
    void ReadBedCapacity( xml::xistream& xis );
    void UpdateDictionary();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_TreatmentCapacities;
    //@}

public:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver_;
    const kernel::Controllers*      controllers_;
    gui::PropertiesDictionary&   dictionary_;
    const kernel::Entity_ABC&       owner_;
    int                             serializableModes_;
    int                             doctors_;
    std::string                     referenceID_;
    T_TreatmentCapacities           capacities_;
    bool                            bUrban_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
