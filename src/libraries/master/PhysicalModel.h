// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PhysicalModel_h_
#define __PhysicalModel_h_

namespace master
{
class DataManager;
class Config;

// =============================================================================
/** @class  PhysicalModel
    @brief  PhysicalModel
*/
// Created: NLD 2007-01-29
// =============================================================================
class PhysicalModel
{
public:
    //! @name Constructors/Destructor
    //@{
             PhysicalModel( const DataManager& dataManager, const Config& config, const std::string& name );
    virtual ~PhysicalModel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhysicalModel( const PhysicalModel& );            //!< Copy constructor
    PhysicalModel& operator=( const PhysicalModel& ); //!< Assignment operator
    //@}

private:
    const std::string name_;
};

}

#endif // __PhysicalModel_h_
