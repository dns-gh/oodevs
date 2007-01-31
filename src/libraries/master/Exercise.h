// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Exercise_h_
#define __Exercise_h_

struct ASN1T_Exercise;

namespace master
{
class DataManager;
class Config;
class Terrain;
class Dataset;
class PhysicalModel;
class Publisher_ABC;

// =============================================================================
/** @class  Exercise
    @brief  Exercise
*/
// Created: NLD 2007-01-29
// =============================================================================
class Exercise
{
public:
    //! @name Constructors/Destructor
    //@{
             Exercise( const DataManager& dataManager, const Config& config, const std::string& name );
    virtual ~Exercise();
    //@}

    //! @name Network
    //@{
    void SendCreation( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Exercise( const Exercise& );            //!< Copy constructor
    Exercise& operator=( const Exercise& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void Send( ASN1T_Exercise& asn ) const;
    //@}

private:
    const std::string name_;

    const Terrain*       pTerrain_;
    const Dataset*       pDataset_;
    const PhysicalModel* pPhysicalModel_;
};

}

#endif // __Exercise_h_
