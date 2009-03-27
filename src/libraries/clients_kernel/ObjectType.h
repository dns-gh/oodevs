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

namespace boost
{
    template<typename T> class shared_ptr;
}

namespace xml 
{ 
    class xibufferstream;
    class xistream; 
};

class ObjectCapacity_ABC;

namespace kernel
{
    class Object_ABC;

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
            ObjectType( xml::xistream& xis, const std::string& type );
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
    bool CanBeReservedObstacle() const;
    bool CanBeValorized() const;
    bool CanBeBypassed() const;

    bool CanBePoint() const;
    bool CanBeLine() const;
    bool CanBePolygon() const;
    bool CanBeCircle() const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectType( const ObjectType& );            //!< Copy constructor
    ObjectType& operator=( const ObjectType& ); //!< Assignement operator
    //@}

    //! @name Helper
    //@{
    void ReadCapacities( const std::string& capacity, xml::xistream& xis );
    xml::xistream* FindCapacity( const std::string& capacity ) const;
    //@}

private:
    //! @name Static members
    //@{
    std::string     type_;
    std::string     symbol_;
    
    T_Capacities    capacities_;

    bool            canBeValorized_;
    bool            canBeBypassed_;
    std::string     geometry_;
    //@}
};

}

#endif // __ObjectType_h_
