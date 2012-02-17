 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Localisation_h_
#define __Localisation_h_

namespace sword
{
    class Location;
    class CoordLatLong;
    enum Location_Geometry;
}

namespace dispatcher
{
    class Automat;
    class Model;

// =============================================================================
/** @class  Localisation
    @brief  Localisation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Localisation
{
public:
    //! @name Constructors/Destructor
    //@{
             Localisation();
    explicit Localisation( const sword::Location& msg );
    virtual ~Localisation();
    //@}

    //! @name Main
    //@{
    void Update( const sword::Location& msg );
    void Send( sword::Location& msg ) const;
    bool IsEmpty() const;
    std::string GetTypeName() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< sword::CoordLatLong > T_PositionVector;
    //@}

private:
    sword::Location_Geometry nType_;
    T_PositionVector points_;
};

}

#endif // __Localisation_h_
