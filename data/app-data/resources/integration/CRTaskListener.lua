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
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

local type_message = 0
local type_rc = 1
local type_event = 2
local type_warning = 3

RC_WithParams = function( RC_Function, type_rc, id, list, emitter )
    if list[1] then
        return RC_Function( type_rc, id, list, emitter )
    else
        return false
    end
end

integration.reportWithSource = function( emitter, id, ... )
    if tableRC[id] then 
        return RC_WithParams( tableRC[id], type_rc, id, {...}, emitter )
    else
        return DEC_RC1( emitter, type_rc, id )
    end
end

DEC_RC = function( ... )
    integration.report( ... )
end

integration.report = function( id, ... )
    integration.reportWithSource( myself, id, ... )
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
        if taskName == myself.currentMission then
            integration.notifyTaskEnded()
        end
    end
    function doneTaskListener:TaskDone()
        integration.notifyTaskEnded()
    end
    masalife.brain.core.registerTaskListener( doneTaskListener )
end

integration.notifyTaskEnded = function( )
    local automat = integration.GetSuperiorKnowledge( meKnowledge )
    if automat then
        integration.SendMessage( "TaskDone", automat, { element = myself }, { type = "dynamic" } )
    end
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

integration.RC_Int_Int = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Int_Int( myself, typeMessage, id, firstList, secondList )
end

integration.RC_Float_Float = function( myself, typeMessage, id, firstList, secondList )
    DEC_RC_Float_Float( myself, typeMessage, id, firstList, secondList )
end

integration.RC_Stage = function( myself, typeMessage, id, list )
    DEC_RC_Stage( myself, typeMessage, id, list )
end

-- Brain debug functions
-- Copied from directia plugin devtools

local formatdottable

local function formatTable( t )
    if masalife.brain.core.class.isInstance( t ) then -- t is a masalife class
        local str = t.tostring ~= nil and t:tostring() or ""
        str = string.len( str ) == 0 and ( "}" ) or ( "|" .. str .. "}" )
        return "{" .. tostring( masalife.brain.core.class.getType( t ) ) .. str
    end
    return ( "{" .. formatdottable( t ) .. "}" )
end

formatdottable = function( t )
    local keys, values = {}, {}
    for k, v in pairs( t ) do
        keys  [ #keys + 1 ]   = type( k ) == 'table' and formatTable( k ) or tostring( k )
        values[ #values + 1 ] = type( v ) == 'table' and formatTable( v ) or tostring( v )
    end
    return "{" .. table.concat( keys, "|" ) .. "}|{" .. table.concat( values, "|" ) ..  "}"
end

local function formatInstanceData( inst )
    return inst.idx .. ' [label="' .. inst.node:format( "%q" ) .. "|" .. formatdottable( inst.parameters ):format( "%q" ).. "|activity = " .. tostring( inst.activity ) .. '"]'
end

local function dottify( graph, res, mem )
    local current = graph
    if not current.idx then
        current.idx, mem.idx = "N" .. mem.idx, mem.idx + 1
        res.nodes[ #res.nodes + 1 ] = formatInstanceData( current )
   end
    
    for _, parent in ipairs( graph.parents or {} ) do
        dottify( parent, res, mem )
        local edge = parent.idx .. "->" .. current.idx
        res.edges[ #res.edges + 1 ], mem.edges[ edge ] = ( not mem.edges[ edge ] or nil ) and edge, edge
    end
end

local function formattodot( graph, name )
    local res = { nodes = {}, edges = {} }
    dottify( graph, res, { edges = {}, idx = 0 } )
    return "digraph ".. name .. " {\nnode [shape=record]\n" .. require"table".concat( res.nodes, "\n" ) .."\n" .. table.concat( res.edges, "\n" ) .. "\n}"
end

local enabledTraces = {}

function ActivateBrainDebug( activate )
    if activate then
        if next( enabledTraces ) == nil then -- table is empty
            masalife.brain.core.getModelData( -- list all skills
                { 
                    NotifySkill = function( self, package, name )
                        local function listener( calltree )
                            if enabledTraces[myself] ~= nil then
                                DEC_Trace( formattodot( calltree, name ) )
                            end
                        end
                        masalife.brain.core.enableTrace( package, listener, "select", 2 )
                    end
                } )
        end
        enabledTraces[myself] = true
    else
        enabledTraces[myself] = nil
        if next( enabledTraces ) == nil then -- table is empty
            masalife.brain.core.getModelData(
                {
                    NotifySkill = function( self, package ) masalife.brain.core.disableTrace( package, listener, "select", 2 ) end
                } )
        end
    end
end
