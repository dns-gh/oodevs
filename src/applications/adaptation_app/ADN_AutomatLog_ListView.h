// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AutomatLog_ListView_h_
#define __ADN_AutomatLog_ListView_h_

#include "ADN_ListView.h"
#include "ADN_Data_ABC.h"
#include "ADN_Equipement_Data.h"
#include <boost/noncopyable.hpp>

class ADN_Automata_Data;

// =============================================================================
// Created: APE 2005-04-04
// =============================================================================
class ADN_AutomatLog_ListView : public ADN_ListView
                              , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_AutomatLog_ListView( ADN_Automata_Data& data, QWidget* pParent = 0 );
    virtual ~ADN_AutomatLog_ListView();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildHeaders();
    void BuildBody   ();
    void InsertCategory( Q3ListViewItem&                         parent,
                         ADN_Composantes_Data::CategoryInfos&   category,
                         ADN_Composantes_Data::ConsumptionItem& conso );
    void InsertCategory( Q3ListViewItem&                         parent,
                         ADN_Composantes_Data::CategoryInfos&   category );
    //@}

private:
    //! @name Columns
    //@{
    enum E_Columns
    {
        eColumnTarget,
        eColumnNbrUnit,
        eColumnNbrComp,
        eColumnContenance,
        eColumnNormalizedConso,
        eColumnMoveConso,
        eColumnMoveAutonomy,
        eColumnEngineStoppedConso,
        eColumnEngineStoppedAutonomy,
        eColumnEngineStartedConso,
        eColumnEngineStartedAutonomy,
        eNbrColumn
    };
    //@}

    //! @name Types
    //@{
    struct ADN_AutomatLog_Entry
    {
        ADN_AutomatLog_Entry()
            : rNbr_( 0. )
            , rNormalizedConsumption_ ( 0. )
            , rMoveQuantityUsedPerHour_( 0. )
            , rMoveAutonomy_( std::numeric_limits< double >::max() )
            , rEngineStoppedQuantityUsedPerHour_( 0. )
            , rEngineStoppedAutonomy_( std::numeric_limits< double >::max() )
            , rEngineStartedQuantityUsedPerHour_( 0. )
            , rEngineStartedAutonomy_( std::numeric_limits< double >::max() )
        {}

        double rNbr_;
        double rNormalizedConsumption_;
        double rMoveQuantityUsedPerHour_;
        double rMoveAutonomy_;
        double rEngineStoppedQuantityUsedPerHour_;
        double rEngineStoppedAutonomy_;
        double rEngineStartedQuantityUsedPerHour_;
        double rEngineStartedAutonomy_;
    };

    typedef std::map< const ADN_Equipement_Data::CategoryInfo*, ADN_AutomatLog_Entry* > T_CategoryEntry;
    typedef T_CategoryEntry::iterator                                                   IT_CategoryEntry;
    typedef T_CategoryEntry::const_iterator                                             CIT_CategoryEntry;
    //@}

    //! @name CategoryEntry tools
    //@{
    double GetMinMoveAutonomy         ( const T_CategoryEntry& entry ) const;
    double GetMinEngineStoppedAutonomy( const T_CategoryEntry& entry ) const;
    double GetMinEngineStartedAutonomy( const T_CategoryEntry& entry ) const;
    void   AddEntryToTotal            ( const T_CategoryEntry& entry, T_CategoryEntry& total, uint nArity );
    void   ClearEntry                 (       T_CategoryEntry& entry );
    void   FillTotalItem              (         Q3ListViewItem& item, const T_CategoryEntry& entry ) const;
    //@}

private:
    ADN_Automata_Data& data_;
    T_CategoryEntry    compTotal_;
    T_CategoryEntry    unitTotal_;
    T_CategoryEntry    automatTotal_;

};

#endif // __ADN_AutomatLog_ListView_h_
