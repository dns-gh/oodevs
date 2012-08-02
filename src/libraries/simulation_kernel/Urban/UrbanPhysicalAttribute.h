// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPhysicalAttribute_h_
#define __UrbanPhysicalAttribute_h_

#include <urban/Extension_ABC.h>

class MIL_UrbanMotivationsVisitor_ABC;

// =============================================================================
/** @class  UrbanPhysicalAttribute
    @brief  UrbanPhysicalAttribute
*/
// Created: JSR 2012-08-01
// =============================================================================
class UrbanPhysicalAttribute : public urban::Extension_ABC
                             , private boost::noncopyable
{
public:
    //! @name types
    //@{
    struct Architecture
    {
        Architecture()
            : height_        ( 20 )
            , floorNumber_   ( 6 )
            , occupation_    ( 0.5f )
            , trafficability_( 0.5f )
            , parkingFloors_ ( 0 )
        {

        }

        std::string roofShape_;
        std::string material_;
        unsigned int height_;
        unsigned int floorNumber_;
        double occupation_;
        double trafficability_;
        unsigned int parkingFloors_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanPhysicalAttribute( xml::xistream& xis );
    virtual ~UrbanPhysicalAttribute();
    //@}

    //! @name Operations
    //@{
    const Architecture& GetArchitecture() const;
    void Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const;
    double GetPathfindCost( double weight ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadXml( const std::string& attribute, xml::xistream& xis );
    void ReadMotivation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    Architecture architecture_;
    std::map< std::string, float > motivations_;
    //@}
};

#endif // __UrbanPhysicalAttribute_h_
