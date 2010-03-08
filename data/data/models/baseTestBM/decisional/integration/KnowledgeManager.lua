-------------------------------------------------------------------------------
-- KnowledgeManager : 
-- Manage query and destroy knowledge when task is finish
-- @author MGD
-- @created 2010-03-03
-- @modified MGD 2010-03-03
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

--- knowledgeManager structure
-- @created 2010-03-03
-- @modified MGD 2010-03-03
knowledgeManager = {}
knowledgeManager.bCanCallStaticQuery = false
knowledgeManager.queries = {}

--- When we start a task we can call queries on static data
-- @author MGD
-- @created 2010-03-03
-- @modified MGD 2010-03-03
knowledgeManager.TaskStarted = function()
  knowledgeManager.bCanCallStaticQuery = true
end
knowledgeManager.StageChanged = function() end
knowledgeManager.TaskFinished = function()
  knowledgeManager.queries = {}
end
knowledgeManager.UpdateDecision = function() knowledgeManager.bCanCallStaticQuery = false end
knowledgeManager.getQueryResult = function( queryName )
  if not knowledgeManager.queries.queryName then
      knowledgeManager.queries.queryName = {}
  end
  return knowledgeManager.queries.queryName
end

masalife.brain.core.registerTaskListener( knowledgeManager )
masalife.brain.core.registerUpdateCallback( knowledgeManager.UpdateDecision )  