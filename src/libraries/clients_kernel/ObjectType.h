// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectType_h_
#define __ObjectType_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xibufferstream;
    class xistream;
};

namespace kernel
{

// =============================================================================
/** @class  ObjectType
    @brief  ObjectType
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectType( xml::xistream& xis );
    virtual ~ObjectType();
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< xml::xibufferstream > >  T_Capacities;
    typedef T_Capacities::const_iterator                                       CIT_Capacities;
    //@}

public:
    //! @name Operations
    //@{
    const std::string& GetName() const;
    const std::string& GetType() const;
    const std::string& GetSymbol() const;

    CIT_Capacities CapacitiesBegin() const;
    CIT_Capacities CapacitiesEnd() const;

    bool HasBuildableDensity() const;
    bool HasLogistic() const;
    bool HasSpawn() const;
    bool HasMedicalCapacity() const;
    bool IsUrban() const;
    bool CanBeReservedObstacle() const;
    bool CanBeValorized() const;
    bool CanBeBypassed() const;
    bool CanBeTrafficable() const;
    bool CanBePoint() const;
    bool CanBeLine() const;
    bool CanBeRectangle() const;
    bool CanBePolygon() const;
    bool CanBeCircle() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectType( const ObjectType& );            //!< Copy constructor
    ObjectType& operator=( const ObjectType& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void ReadCapacities( const std::string& capacity, xml::xistream& xis );
    void SetValorizable( xml::xistream& xis );
    xml::xistream* FindCapacity( const std::string& capacity ) const;
    //@}

private:
    //! @name Static members
    //@{
    const std::string name_;
    const std::string type_;
    const std::string symbol_;
    const std::string geometry_;
    T_Capacities capacities_;
    bool canBeValorized_;
    bool canBeBypassed_;
    //@}
};

}

#endif // __ObjectType_h_
