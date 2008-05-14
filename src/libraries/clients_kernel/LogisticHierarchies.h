// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_LogisticHierarchies_h_
#define __kernel_LogisticHierarchies_h_

#include "Hierarchies.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Automat_ABC;

// =============================================================================
/** @class  LogisticHierarchiesBase
    @brief  LogisticHierarchiesBase
    // $$$$ AGE 2006-11-21: bouger dans prepa
*/
// Created: AGE 2006-11-21
// =============================================================================
class LogisticHierarchiesBase : public Hierarchies
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchiesBase() {};
    virtual ~LogisticHierarchiesBase() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const = 0;
    virtual void Load( xml::xistream& xis, const Automat_ABC* superior ) = 0;
    //@}
};

class MaintenanceHierarchies : public LogisticHierarchiesBase
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceHierarchies() {};
    virtual ~MaintenanceHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "maintenance"; };
    //@}
};

class MedicalHierarchies : public LogisticHierarchiesBase
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalHierarchies() {};
    virtual ~MedicalHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "medical"; };
    //@}
};

class SupplyHierarchies : public LogisticHierarchiesBase
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyHierarchies() {};
    virtual ~SupplyHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "supply"; };
    //@}
};

class TC2Hierarchies : public LogisticHierarchiesBase
{

public:
    //! @name Constructors/Destructor
    //@{
             TC2Hierarchies() {};
    virtual ~TC2Hierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual QString GetLinkType() const { return "tc2"; };
    //@}
};


}

#endif // __kernel_LogisticHierarchies_h_
