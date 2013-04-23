// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SymbolHierarchy_h_
#define __SymbolHierarchy_h_

#include "SymbolHierarchy_ABC.h"

namespace kernel
{
    class SymbolFactory;

// =============================================================================
/** @class  SymbolHierarchy
    @brief  SymbolHierarchy
*/
// Created: LGY 2011-07-28
// =============================================================================
class SymbolHierarchy : public SymbolHierarchy_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolHierarchy( const kernel::SymbolFactory& factory );
    virtual ~SymbolHierarchy();
    //@}

public:
    //! @name Operations
    //@{
    virtual const std::string& GetValue() const;
    virtual std::string GetStaticValue() const;
    virtual std::string GetMoveValue() const;
    virtual void OverrideValue( const std::string& value );
    virtual void MergeSymbol( const std::string& symbol, bool original = false );
    virtual void ResetSymbol( const std::string& symbol );
    virtual void PrepareForMerge();
    virtual void Reset();
    virtual bool IsOverriden() const;
    virtual void UpdateKarma( const Karma& karma );
    void SetOverriden( bool value );
    //@}

private:
    //! @name Member data
    //@{
    std::string symbol_;
    std::string computedSymbol_;
    std::string originalSymbol_;
    bool overriden_;
    const kernel::SymbolFactory& factory_;
    //@}
};

}

#endif // __SymbolHierarchy_h_
