// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionCapacity_h_
#define __ConstructionCapacity_h_

class PHY_ConsumptionType;
class ObjectPrototype;

// =============================================================================
/** @class  ConstructionCapacity
    @brief  ConstructionCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class ConstructionCapacity
{
public:
    //! @name Types
    //@{
    enum E_UnitType { eRaw, eDensity };
    //@}

public:
    //! @name Constructor / destructor
    //@{
             ConstructionCapacity( ObjectPrototype& prototype, xml::xistream& xis );
    virtual ~ConstructionCapacity();
    //@}

    //! @name Operations
    //@{
    void AddCapacity( const std::string& capacity, xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy / Assigment
    //@{
    ConstructionCapacity( const ConstructionCapacity& );
    ConstructionCapacity& operator=( const ConstructionCapacity& );
    //@}

private:        
    //! @name Member data
    //@{
    const PHY_ConsumptionType* default_;
    E_UnitType unit_type_;
    ObjectPrototype& prototype_;
    //@}
};

#endif // __ConstructionCapacity_h_