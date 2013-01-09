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

RC_WithParams = function( RC_Function, type_rc, id, list )
    if list[1] then
        return RC_Function( type_rc, id, list )
    else
        return false
    end
end

DEC_RC = function( ... )
    integration.report( ... )
end

integration.report = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_rc, id, {...} )
    else
        return DEC_RC1( myself, type_rc, id )
    end
end


integration.displayTrace = function ( stringMessage )
    DEC_Trace( stringMessage )
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
    integration.report( ... )
end

integration.pionRC = function ( ... )
    if not DEC_Agent_AutomateEstEmbraye() then
        integration.report( ... )
    else
        DEC_Message( ... )
    end
end

function RegisterTaskListener()
    local taskListener = {
        stage = {},
        OnNewTick = function( self )
            myself.newMission = false
            if myself.stealthTick == 2 then -- I wait 2 ticks
                DEC_Perception_Furtivite( 1 )  -- I become visible
                myself.lastStealth = 1
                myself.wantedVisible = false
                myself.stealthTick = 0
            end
            if myself.wantedVisible then -- I want to be visible, I had to wait
                myself.stealthTick = myself.stealthTick or 0
                myself.stealthTick = myself.stealthTick + 1
            end
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
            myself.safetyAttitude = eAmbianceMission_None
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

integration.RC_String = function( myself, typeMessage, id, list )
    DEC_RC_String( myself, typeMessage, id, list )
end

integration.RC_ObjectKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_ObjectKnowledge( myself, typeMessage, id, list )
end

integration.RC_AgentKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_AgentKnowledge( myself, typeMessage, id, list )
end

integration.RC_Pion_Automate = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Pion_Automate( myself, typeMessage, id, firstList, secondList )
end

integration.RC_Pion_Pion = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Pion_Pion( myself, typeMessage, id, firstList, secondList )
end

integration.RC_Id = function( myself, typeMessage, id, list )
    DEC_RC_Id( myself, typeMessage, id, list )
end

integration.RC_PopulationKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_PopulationKnowledge( myself, typeMessage, id, list )
end

integration.RC_Float = function( myself, typeMessage, id, list )
    DEC_RC_Float( myself, typeMessage, id, list )
end