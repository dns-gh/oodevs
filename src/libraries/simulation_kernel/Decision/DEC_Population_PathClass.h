// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Population_PathClass_h_
#define __DEC_Population_PathClass_h_

namespace xml
{
    class xistream;
}

class MIL_ObjectType_ABC;

// =============================================================================
/** @class  DEC_Population_PathClass
    @brief  DEC_Population_PathClass
*/
// Created: SBO 2006-03-23
// =============================================================================
class DEC_Population_PathClass
{
public:
    //! @name Manager
    //@{
    static       void                      Initialize  ( xml::xistream& xis );
    static       void                      Terminate   ();
    static const DEC_Population_PathClass& GetPathClass( const std::string& type );
    //@}

    //! @name Constructors/Destructor
    //@{
             DEC_Population_PathClass( xml::xistream& xis, const DEC_Population_PathClass* pCopyFrom = 0 );
    virtual ~DEC_Population_PathClass();
    //@}

    //! @name Accessors
    //@{
    double GetCostOutsideOfChanneling() const;
    double GetChannelingRange        () const;
    bool   AvoidObjects              () const;
    double GetThreshold              () const;
    double GetObjectCost             ( const MIL_ObjectType_ABC& objectType ) const;
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadPopulationRule( xml::xistream& xis );
    //@}

private:
    //! @name Initialization
    //@{
    void ReadObjectsCost( xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    void ReadObject( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< double > T_ObjectCosts;
    typedef std::map< const std::string, DEC_Population_PathClass* > T_Rules;
    //@}

    //! @name Member data
    //@{
    double rCostOutsideOfChanneling_;
    double rChannelingRange_;
    bool bAvoidObjects_;
    double rObstructionThreshold_;
    T_ObjectCosts objectCosts_;
    //@}

    //! @name Statics
    //@{
    static T_Rules rules_;
    //@}
};

#include "DEC_Population_PathClass.inl"

#endif // __DEC_Population_PathClass_h_
