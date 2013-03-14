// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateExercise_h_
#define __CreateExercise_h_

namespace tools
{
    class GeneralConfig;
}

namespace frontend
{
    struct ExerciseCopyParameters
    {
        ExerciseCopyParameters() {}
        virtual ~ExerciseCopyParameters() {}

        tools::Path from_;
        tools::Path to_;
        tools::Path checkpoint_;
        tools::Path terrain_;
        tools::Path model_;
        tools::Path physical_;
        const QStandardItemModel* itemModel_;
    };

    void CreateExercise( const tools::GeneralConfig& config, const tools::Path& name,
                         const tools::Path& terrain, const tools::Path& model, const tools::Path& physical = tools::Path() );
    void EditExerciseParameters( const tools::GeneralConfig& config, const tools::Path& name,
                         const tools::Path& terrain, const tools::Path& model, const tools::Path& physical = tools::Path() );

    void CreateExerciseAsCopyOf( const tools::GeneralConfig& config, const ExerciseCopyParameters& parameters );
    void CreateExerciseAsCopyOfCheckpoint( const tools::GeneralConfig& config, const ExerciseCopyParameters& params );
}

#endif // __CreateExercise_h_
