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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AutomatLogCategory_ListView.h $
// $Author: Ape $
// $Modtime: 20/04/05 15:56 $
// $Revision: 2 $
// $Workfile: ADN_AutomatLogCategory_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_AutomatLogCategory_ListView_h_
#define __ADN_AutomatLogCategory_ListView_h_

#include "ADN_ListView.h"
#include "ADN_Data_ABC.h"
#include "ADN_Resources_Data.h"
#include "ADN_Units_Data.h"
#include <boost/noncopyable.hpp>

class ADN_Automata_Data;
class ADN_Rich_ListViewItem;

// =============================================================================
// Created: APE 2005-04-04
// =============================================================================
class ADN_AutomatLogCategory_ListView : public ADN_ListView
                                      , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_AutomatLogCategory_ListView( ADN_Automata_Data& data, QWidget* pParent = 0 );
    virtual ~ADN_AutomatLogCategory_ListView();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildHeaders();
    void BuildBody   ();
    void FillComponentItem( ADN_Rich_ListViewItem&             item,
                            ADN_Resources_Data::CategoryInfo& category,
                            ADN_Units_Data::ComposanteInfos&   comp );
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
    struct Entry
    {
    public:
        Entry()
            : rNbr_( 0. )
            , rNormalizedConsumption_ ( 0. )
            , rMoveQuantityUsedPerHour_( 0. )
            , rMoveAutonomy_( std::numeric_limits< double >::max() )
            , rEngineStoppedQuantityUsedPerHour_( 0. )
            , rEngineStoppedAutonomy_( std::numeric_limits< double >::max() )
            , rEngineStartedQuantityUsedPerHour_( 0. )
            , rEngineStartedAutonomy_( std::numeric_limits< double >::max() )
        {}

        void Clear()
        {
            rNbr_                              = 0.;
            rNormalizedConsumption_            = 0.;
            rMoveQuantityUsedPerHour_          = 0.;
            rMoveAutonomy_                     = std::numeric_limits< double >::max();
            rEngineStoppedQuantityUsedPerHour_ = 0.;
            rEngineStoppedAutonomy_            = std::numeric_limits< double >::max();
            rEngineStartedQuantityUsedPerHour_ = 0.;
            rEngineStartedAutonomy_            = std::numeric_limits< double >::max();
        };

        double rNbr_;
        double rNormalizedConsumption_;
        double rMoveQuantityUsedPerHour_;
        double rMoveAutonomy_;
        double rEngineStoppedQuantityUsedPerHour_;
        double rEngineStoppedAutonomy_;
        double rEngineStartedQuantityUsedPerHour_;
        double rEngineStartedAutonomy_;
    };
    typedef struct Entry T_Entry;
    //@}

    //! @name CategoryEntry tools
    //@{
    void AddEntryToTotal( const T_Entry& entry, T_Entry& total, uint nArity );
    void FillTotalItem  ( ADN_Rich_ListViewItem& item, const T_Entry& total ) const;
    //@}

private:
    ADN_Automata_Data& data_;
    T_Entry            compTotal_;
    T_Entry            unitTotal_;
    T_Entry            automatTotal_;
    T_Entry            categoryTotal_;
    T_Entry            dotationTotal_;

};

#endif // __ADN_AutomatLogCategory_ListView_h_
