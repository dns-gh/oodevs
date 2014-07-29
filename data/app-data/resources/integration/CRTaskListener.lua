-------------------------------------------------------------------
---- CRTASKLISTENER INTERFACE IMPLEMENTATION
-------------------------------------------------------------------

local type_message = 0
local type_rc = 1
local type_event = 2
local type_warning = 3

--- Call the specific function for sending a message with parameters
-- @param RC_Function, the method used for sending a message
-- @param type_rc, the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the list of parameters. If it's not a list, return false
-- @param emitter the sending agent
RC_WithParams = function( RC_Function, type_rc, id, list, emitter )
    if list[1] then
        return RC_Function( type_rc, id, list, emitter )
    else
        return false
    end
end

--- Sends a message
-- @see RC_WithParams
-- Any additional parameter will be used as parameters to the report.
-- @param emitte, the sending agent
-- @param id the identifier of the message
integration.reportWithSource = function( emitter, id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_rc, id, {...}, emitter )
    else
        return DEC_RC1( emitter, type_rc, id )
    end
end

--- Sends a message
-- @see integration.reportWithSource
-- Any additional parameter will be used as parameters to the report.
-- @param id the identifier of the message
integration.report = function( id, ... )
    integration.reportWithSource( myself, id, ... )
end

--- Deprecated : use integration.report
--- Sends a message
-- @see integration.report
-- Any additional parameter will be used as parameters to the report.
DEC_RC = integration.report

--- Displays a trace
-- @param stringMessage, String, the message to display
integration.displayTrace = function ( stringMessage )
    DEC_Trace( stringMessage )
end

--- Sends a message of type "message"
-- @see RC_WithParams
-- Any additional parameter will be used as parameters to the report.
-- @param id the identifier of the message
DEC_Message = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_message, id, {...} )
    else
        return DEC_RC1( myself, type_message, id )
    end
end

--- Sends a message of type "warning"
-- @see RC_WithParams
-- Any additional parameter will be used as parameters to the report.
-- @param id the identifier of the message
DEC_Warning = function( id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_warning, id, {...} )
    else
        return DEC_RC1( myself, type_warning, id )
    end
end

--- Deprecated : use integration.report
--- Sends a message
-- @see integration.report
-- Any additional parameter will be used as parameters to the report.
integration.genericRC = integration.report

--- Sends a message
-- @see integration.report
-- @see DEC_Message
-- Any additional parameter will be used as parameters to the report.
integration.pionRC = function ( ... )
    if not DEC_Agent_AutomateEstEmbraye() then
        integration.report( ... )
    else
        DEC_Message( ... )
    end
end

--- Listen the state of the task and execute method for each state
-- OnNewTick; TaskStarted; StageChanged; Cleanup; TaskFinished; TaskDone
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
                reportFunction( eRC_DebutMission )
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
                    reportFunction( eRC_BM_FinPhase, self.stage[ taskName ] )
                end
                self.stage[ taskName ] = label
                reportFunction( eRC_BM_DebutPhase, label )
            end
        end,
        Cleanup = function( self )
            -- Clear knowledge cache (queries)
            myself.engineerObjectsOnPath = {}
            myself.blocked = nil
            myself.safetyAttitude = eAmbianceMission_None
        end,
        TaskFinished = function( self, taskName )
            reportFunction( eRC_BM_FinPhase, self.stage[ taskName ] )

            meKnowledge.currentTask = nil
            if self.main == taskName then
                self.main = nil
                reportFunction( eRC_FinMission )
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

--- Listen the done state of the task depending of the cause
-- TaskStarted; StageChanged; TaskFinished; TaskDone
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
        integration.notifyTaskEnded()
    end
    masalife.brain.core.registerTaskListener( doneTaskListener )
end

--- Notify the automat that the task is done
integration.notifyTaskEnded = function( )
    local automat = integration.GetSuperiorKnowledge( meKnowledge )
    if automat then
        integration.SendMessage( "TaskDone", automat, { element = myself }, { type = "dynamic" } )
    end
end

--- Emits a message with a String parameter
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the String parameter
integration.RC_String = function( myself, typeMessage, id, list )
    DEC_RC_String( myself, typeMessage, id, list )
end

--- Emits a message with an Object knowledge parameter
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Simulation Object knowledge parameter
integration.RC_ObjectKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_ObjectKnowledge( myself, typeMessage, id, list )
end

--- Emits a message with an Agent parameter
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Simulation Agent knowledge parameter
integration.RC_AgentKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_AgentKnowledge( myself, typeMessage, id, list )
end

--- Emits a message with an Agent and an Automat parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param firstList the Simulation Agent parameter
-- @param secondList the Simulation Automat parameter
integration.RC_Pion_Automate = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Pion_Automate( myself, typeMessage, id, firstList, secondList )
end

--- Emits a message with an Agent and an Agent parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the number of the message
-- @param firstList the Simulation Agent parameter
-- @param secondList the Simulation Agent parameter
integration.RC_Pion_Pion = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Pion_Pion( myself, typeMessage, id, firstList, secondList )
end

--- Emits a message with an Agent parameter
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the number of the message
-- @param list the Simulation Agent parameter
integration.RC_Pion = function( myself, typeMessage, id, list )
    DEC_RC_Pion( myself, typeMessage, id, list )
end

--- Emits a message with a Crowd parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Simulation Crowd parameter
integration.RC_Id = function( myself, typeMessage, id, list )
    DEC_RC_Id( myself, typeMessage, id, list )
end

--- Emits a message with a Population parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Simulation Population
integration.RC_PopulationKnowledge = function( myself, typeMessage, id, list )
    DEC_RC_PopulationKnowledge( myself, typeMessage, id, list )
end

--- Emits a message with a Float parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Float
integration.RC_Float = function( myself, typeMessage, id, list )
    DEC_RC_Float( myself, typeMessage, id, list )
end

--- Emits a message with an Integer and an Integer parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param firstList the Integer parameter
-- @param secondList the Integer parameter
integration.RC_Int_Int = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Int_Int( myself, typeMessage, id, firstList, secondList )
end

--- Emits a message with an Agent knowledge and an Integer parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param firstList the Simulation Agent knowledge parameter
-- @param secondList the Integer parameter
integration.RC_AgentKnowledge_Int = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_AgentKnowledge_Int( myself, typeMessage, id, firstList, secondList )
end

--- Emits a message with a Float and a Float parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param firstList the Float parameter
-- @param secondList the Float parameter
integration.RC_Float_Float = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Float_Float( myself, typeMessage, id, firstList, secondList )
end

--- Emits a message with a Stage parameters
-- @param myself the message sender
-- @param typeMessage the type of the message (type_message; type_rc; type_event; type_warning)
-- @param id the identifier of the message
-- @param list the Stage
integration.RC_Stage = function( myself, typeMessage, id, list )
    DEC_RC_Stage( myself, typeMessage, id, list )
end

--- Stop all tasks, main task and fragOrder
integration.stopTasks = function( )
    masalife.brain.core.stopTasks()
    myself.currentMission = nil
    myself.taskParams = {}
end


