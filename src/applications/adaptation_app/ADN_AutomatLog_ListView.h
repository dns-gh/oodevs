// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AutomatLog_ListView.h $
// $Author: Ape $
// $Modtime: 20/04/05 15:56 $
// $Revision: 2 $
// $Workfile: ADN_AutomatLog_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_AutomatLog_ListView_h_
#define __ADN_AutomatLog_ListView_h_

#include "ADN_ListView.h"

#include "ADN_Data_ABC.h"
#include "ADN_Equipement_Data.h"

class ADN_Automata_Data;
class QListViewItem;

// =============================================================================
// Created: APE 2005-04-04
// =============================================================================
class ADN_AutomatLog_ListView : public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_AutomatLog_ListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_AutomatLog_ListView( ADN_Automata_Data& data, QWidget* pParent = 0 );
    ~ADN_AutomatLog_ListView();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildHeaders();
    void BuildBody   ();
    void InsertCategory( QListViewItem&                         parent,
                         ADN_Composantes_Data::CategoryInfos&   category,
                         ADN_Composantes_Data::ConsumptionItem& conso );
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
    void   FillTotalItem              (         QListViewItem& item, const T_CategoryEntry& entry ) const;
    //@}

private:
    ADN_Automata_Data& data_;
    T_CategoryEntry    compTotal_;
    T_CategoryEntry    unitTotal_;
    T_CategoryEntry    automatTotal_;

};

#endif // __ADN_AutomatLog_ListView_h_
