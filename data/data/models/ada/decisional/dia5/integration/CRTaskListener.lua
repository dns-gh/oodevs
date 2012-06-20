-------------------------------------------------------------------------------
-- KnowledgeManager :
-- Manage query and destroy knowledge when task is finish
-- @author MGD
-- @created 2010-03-24
-- @modified MGD 2010-10-28
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
-------------------------------------------------------------------------------

local type_message = 0
local type_rc = 1
local type_event = 2
local type_warning = 3

tableRC = {}
tableRC[ eRC_BM_DebutMission ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_FinMission ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_DebutPhase ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_FinPhase ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutAnimationObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinAnimationObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutDegagement ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutTravauxObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTravauxObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TirDirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TirIndirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_EnnemiDetruit ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_EnnemiTactiquementDetruit ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTirDirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTirIndirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutNeutralisationCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinNeutralisationCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObjetDejaOccupe ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ReussiteRenforcement ] = function( typeMessage, id, list ) return DEC_RC_Pion_Automate( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_EchecRenforcement ] = function( typeMessage, id, list ) return DEC_RC_Pion_Automate( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_ReussiteRenforcementPion ] = function( typeMessage, id, list ) return DEC_RC_Pion_Pion( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_EchecRenforcementPion ] = function( typeMessage, id, list ) return DEC_RC_Pion_Pion( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_SurLima ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_AttitudePopulation ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_MissionImpossibleReason ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObjetNonContournable ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_LeaveLima ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutValorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinValorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutDevalorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinDevalorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObstacleEnAttenteActivation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_criticalIntelligence ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_RiposteContrePopulation ] = function( typeMessage, id, list ) return DEC_RC_PopulationKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TerroristCaptured ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TakeHostage ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TimeInUndergroundNetwork ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_PreparationMission ] = function( typeMessage, id, list ) return DEC_RC_Float( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_LimaParameterNotPresent ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TerroristDropped ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObstacleNonActivable ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ActivationObstacleDeManoeuvre ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end

RC_WithParams = function( RC_Function, type_rc, id, list )
    if list[1] then
        return RC_Function( type_rc, id, list )
    else
        return false
    end
end

DEC_RC = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_rc, id, {...} )
    else
        return DEC_RC1( myself, type_rc, id )
    end
end

DEC_Message = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_message, id, {...} )
    else
        return DEC_RC1( myself, type_message, id )
    end
end

DEC_Warning = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_warning, id, {...} )
    else
        return DEC_RC1( myself, type_warning, id )
    end
end

integration.genericRC = function ( ... ) 
    DEC_RC( ... )
end

integration.pionRC = function ( ... )
    if not DEC_Agent_AutomateEstEmbraye() then
        DEC_RC( ... )
    else
        DEC_Message( ... )
    end
end

function RegisterTaskListener()
    local taskListener = {
        stage = {},
        OnNewTick = function( self )
            myself.newMission = false
        end,
        TaskStarted = function( self, taskName )
            if myself.currentMission == taskName then
                self.main = taskName
                meKnowledge:RC( eRC_DebutMission )
                myself.newMission = true
            end
            local fun = function( task )
                if task.name == taskName then
                    return task
                end
            end
            meKnowledge.currentTask =  masalife.brain.knowledge.me.tasks:accept( fun )
        end,
        StageChanged = function( self, taskName, id, label)
            if myself.currentMission == taskName then
                if self.stage[ taskName ] then
                    meKnowledge:RC( eRC_BM_FinPhase, self.stage[ taskName ] )
                end
                self.stage[ taskName ] = label
                meKnowledge:RC( eRC_BM_DebutPhase, label )
            end
        end,
        Cleanup = function( self )
            -- Clear knowledge cache (queries)
            myself.engineerObjectsOnPath = {}
            myself.blocked = nil
        end,
        TaskFinished = function( self, taskName )
            meKnowledge:RC( eRC_BM_FinPhase, self.stage[ taskName ] )
            meKnowledge.currentTask = nil
            if self.main == taskName then
                self.main = nil
                meKnowledge:RC( eRC_FinMission )
                DEC_FinMission()
            end
            self.stage[ taskName ] = nil
            self:Cleanup()
        end,
        TaskDone = function( self )
            self:Cleanup()
        end,
    }
    masalife.brain.core.registerTaskListener( taskListener )
    masalife.brain.core.registerUpdateCallback( taskListener.OnNewTick )
end

function RegisterDoneTaskListener()
    local doneTaskListener = {}
    doneTaskListener.stage = {}
    function doneTaskListener:TaskStarted( taskName )
        -- NOTHING
    end
    function doneTaskListener:StageChanged( taskName, id, label)
        -- NOTHING
    end
    function doneTaskListener:TaskFinished( taskName )
        -- NOTHING
    end
    function doneTaskListener:TaskDone()
        local automat = integration.GetSuperiorKnowledge( meKnowledge )
        if automat then
            integration.SendMessage( "TaskDone", automat, { element = myself }, { type = "dynamic" } )
        end
    end
    masalife.brain.core.registerTaskListener( doneTaskListener )
end
