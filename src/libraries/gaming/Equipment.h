// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipment_h_
#define __Equipment_h_

namespace sword
{
    class EquipmentDotations_EquipmentDotation;
    class LogMaintenanceHandlingUpdate;
    class LogMaintenanceHandlingCreation;
}

namespace kernel
{
    class EquipmentType;
}

// =============================================================================
/** @class  Equipment
    @brief  Equipment
*/
// Created: AGE 2006-02-21
// =============================================================================
class Equipment
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Equipment( const kernel::EquipmentType& type );
             Equipment( const Equipment& rhs );
    virtual ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::EquipmentDotations_EquipmentDotation& message );
    QString GetName() const;
    unsigned Total() const;
    void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    void DeleteMaintenanceConsign( int id );
    void UpdateMaintenanceConsign( const sword::LogMaintenanceHandlingUpdate& message );
    const std::vector< int >& GetBreakdowns() const;
    std::vector< int > GetBreakdownsInTreatment( bool isDiagnosed ) const;
    //@}

    //! @name Operators
    //@{
    Equipment operator+( const Equipment& rhs ) const;
    Equipment operator-( const Equipment& rhs ) const;
    Equipment operator-() const;
    Equipment& operator=( const Equipment& rhs );
    //@}

private:
    //! @name types
    //@{
    struct Breakdown
    {
        int type_;
        bool diagnosed_;
    };
    //@}

public:
    //! @name Member data
    //@{
    const kernel::EquipmentType& type_;
    int                          available_;
    int                          unavailable_;
    int                          repairable_;
    int                          onSiteFixable_;
    int                          inMaintenance_;
    int                          prisonners_;
private:
    std::vector< int >           breakdowns_;
    std::map< int, Breakdown >   consigns_;
    //@}
};

#endif // __Equipment_h_
