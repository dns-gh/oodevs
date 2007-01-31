// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Terrain_h_
#define __Terrain_h_

struct ASN1T_Terrain;

namespace master
{
class DataManager;
class Config;
class Publisher_ABC;

// =============================================================================
/** @class  Terrain
    @brief  Terrain
*/
// Created: NLD 2007-01-29
// =============================================================================
class Terrain
{
public:
    //! @name Constructors/Destructor
    //@{
             Terrain( const DataManager& dataManager, const Config& config, const std::string& name );
    virtual ~Terrain();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}

    //! @name Network
    //@{
    void SendCreation( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Terrain( const Terrain& );            //!< Copy constructor
    Terrain& operator=( const Terrain& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void Send( ASN1T_Terrain& asn ) const;
    //@}

private:
    const std::string name_;
};

}

#endif // __Terrain_h_
