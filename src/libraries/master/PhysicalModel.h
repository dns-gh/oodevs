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

struct ASN1T_PhysicalModel;

namespace master
{
class Dataset;
class Config;
class Publisher_ABC;

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
             PhysicalModel( const Dataset& dataset, const Config& config, const std::string& name );
    virtual ~PhysicalModel();
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
    PhysicalModel( const PhysicalModel& );            //!< Copy constructor
    PhysicalModel& operator=( const PhysicalModel& ); //!< Assignment operator
    //@}

    //! @name Tools
    //@{
    void Send( ASN1T_PhysicalModel& asn ) const;
    //@}

private:
    const Dataset&    dataset_;
    const std::string name_;
};

}

#endif // __PhysicalModel_h_
