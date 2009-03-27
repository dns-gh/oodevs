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


class ConstructionCapacity
{
public:
    //! @name 
    //@{
    enum E_UnitType { eRaw, eDensity };
    //@}
public:
    //! @name Constructor / destructor
    //@{
    explicit ConstructionCapacity( ObjectPrototype& prototype, xml::xistream& xis ) ;
    virtual ~ConstructionCapacity() ;
    //@}

    //! @name 
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
    //! @name Data
    //@{
    const PHY_ConsumptionType* default_;
    E_UnitType unit_type_;
    ObjectPrototype& prototype_;
    //@}
};

#endif // __ConstructionCapacity_h_