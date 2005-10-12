// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_InfoPanel_ABC.h $
// $Author: Age $
// $Modtime: 6/04/05 11:56 $
// $Revision: 2 $
// $Workfile: MOS_InfoPanel_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_InfoPanel_ABC_h_
#define __MOS_InfoPanel_ABC_h_

#include "MOS_SelectedElement.h"

class MOS_Agent;
class MOS_Population;
class MOS_Agent_ABC;

// =============================================================================
/** @class  MOS_InfoPanel_ABC
    @brief  MOS_InfoPanel_ABC
*/
// Created: AGE 2005-04-05
// =============================================================================
class MOS_InfoPanel_ABC : public QScrollView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_InfoPanel_ABC( QWidget* pParent );
    virtual ~MOS_InfoPanel_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void SetSelection( MOS_SelectedElement& item );
    virtual void insertChild ( QObject*             pObj );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection() = 0;
    virtual void OnAgentUpdated  ( MOS_Agent&      agent  );
    virtual void OnObjectUpdated ( MOS_Object_ABC& object );
    virtual void OnPopulationUpdated  ( MOS_Population& population );

    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename Cont1 >
    void Suit( Cont1& destCont, unsigned int nSize, QListView* pView )
    {
        if( ! nSize )
        {
            pView->hide();
            return;
        }
        pView->show();

        while( destCont.size() < nSize )
        {
            destCont.push_back( new MT_ListViewItem( pView ) );
            destCont.back()->SetBackground( destCont.size() % 2 == 0 ? QColor( 255, 255, 255 ) : QColor( 230, 230, 255 ) );
        }
        while( destCont.size() > nSize )
        {
            delete destCont.back();
            destCont.pop_back();
        }
    }

    bool     ShouldDisplay( MOS_Agent& agent );
    bool     ShouldDisplay( MOS_Population& population );
    bool     ShouldDisplay( MOS_Object_ABC& object );
    QLayout* layout       ();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_InfoPanel_ABC( const MOS_InfoPanel_ABC& );            //!< Copy constructor
    MOS_InfoPanel_ABC& operator=( const MOS_InfoPanel_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    QVBox* pBox_;
    MOS_SelectedElement selectedItem_;
    bool bVisible_;
    //@}
};

#endif // __MOS_InfoPanel_ABC_h_
