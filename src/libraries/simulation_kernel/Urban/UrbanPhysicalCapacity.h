// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPhysicalCapacity_h_
#define __UrbanPhysicalCapacity_h_

#include "Entities/Objects/ObjectCapacity_ABC.h"

namespace sword
{
    class UrbanAttributes;
}

// =============================================================================
/** @class  UrbanPhysicalCapacity
    @brief  UrbanPhysicalCapacity
*/
// Created: JSR 2012-08-01
// =============================================================================
class UrbanPhysicalCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPhysicalCapacity();
    explicit UrbanPhysicalCapacity( xml::xistream& xis );
    virtual ~UrbanPhysicalCapacity();
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetHeight() const;
    unsigned int GetFloorNumber() const;
    double GetOccupation() const;
    double GetTrafficability() const;
    const std::string& GetMaterial() const;
    const std::string& GetRoofShape() const;
    const std::map< std::string, float >& GetMotivations() const;
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive >
    void serialize( Archive& file, const unsigned int );
    void WriteUrban( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanPhysicalCapacity( const UrbanPhysicalCapacity& );            //!< Copy constructor
    UrbanPhysicalCapacity& operator=( const UrbanPhysicalCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadXml( const std::string& attribute, xml::xistream& xis );
    void ReadMotivation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int height_;
    unsigned int floorNumber_;
    double occupation_;
    double trafficability_;
    unsigned int parkingFloors_;
    std::string roofShape_;
    std::string material_;
    std::map< std::string, float > motivations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UrbanPhysicalCapacity )

#endif // __UrbanPhysicalCapacity_h_
