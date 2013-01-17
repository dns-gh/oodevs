// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef PropertiesComparator_h
#define PropertiesComparator_h

#include "PropertyDisplayer.h"

// =============================================================================
/** @class  PropertiesComparator
    @brief  Properties comparator
*/
// Created: LGY 2013-01-16
// =============================================================================
class PropertiesComparator : public PropertyDisplayer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesComparator( const kernel::CoordinateConverter_ABC& converter );
    virtual ~PropertiesComparator();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsValid() const;
    virtual void Clear();
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

private:
    //! @name Member data
    //@{
    std::vector< QString > texts_;
    //@}
};

#endif // PropertiesComparator_h
