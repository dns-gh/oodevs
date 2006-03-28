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

class DEC_PathType;
class MIL_Population;

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
    static void                 Initialize  ( MIL_InputArchive& archive );
    static void                 Terminate   ();
    static const DEC_Population_PathClass& GetPathClass( const std::string& type );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Population_PathClass( MIL_InputArchive& archive, const DEC_Population_PathClass* pCopyFrom = 0 );
    virtual ~DEC_Population_PathClass();
    //@}

    //! @name Operations
    //@{
    double GetCostOutsideOfChanneling() const;
    double GetChannelingRange        () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const std::string, DEC_Population_PathClass* > T_Rules;
    //@}

private:
    //! @name Member data
    //@{
    double rCostOutsideOfChanneling_;
    double rChannelingRange_;
    //@}

private:
    //! @name Statics
    //@{
    static T_Rules rules_;
    //@}
};

#endif // __DEC_Population_PathClass_h_
