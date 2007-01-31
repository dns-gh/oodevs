// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dataset_h_
#define __Dataset_h_

#include "ModelsContainer.h"
#include "PhysicalModel.h"

struct ASN1T_Dataset;

namespace master
{
class DataManager;
class Config;

// =============================================================================
/** @class  Dataset
    @brief  Dataset
*/
// Created: NLD 2007-01-29
// =============================================================================
class Dataset
{
public:
    //! @name Constructors/Destructor
    //@{
             Dataset( const DataManager& dataManager, const Config& config, const std::string& name );
    virtual ~Dataset();
    //@}

    //! @name Accessors
    //@{
    const ModelsContainer< std::string, PhysicalModel >& GetPhysicalModels() const;
    const std::string&                                   GetName          () const;
    //@}

    //! @name Network
    //@{
    void SendCreation( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Dataset( const Dataset& );            //!< Copy constructor
    Dataset& operator=( const Dataset& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void Send( ASN1T_Dataset& asn ) const;
    //@}

private:
    const std::string name_;

    ModelsContainer< std::string, PhysicalModel > physicalModels_;
};

}

#endif // __Dataset_h_
