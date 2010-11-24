<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0"
    exclude-result-prefixes="xsi">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" standalone="no"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="parameter">
    <xsl:choose>
      <xsl:when test="@type='Bool'">
        <parameter name="{@name}" type="Boolean" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='Automate' or @type='AutomateBM'">
        <parameter name="{@name}" type="Automat" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='PopulationKnowledge'">
        <parameter name="{@name}" type="CrowdKnowledge" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='DotationType'">
        <parameter name="{@name}" type="ResourceType" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='genobject' or  @type='genobjectBM'">
        <parameter name="{@name}" type="plannedwork">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='obstacle'">
        <parameter name="{@name}" type="plannedwork" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='UrbanBlock' or @type='UrbanBlockBM'">
        <parameter name="{@name}" type="UrbanKnowledge" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='Numeric'">
        <parameter name="{@name}" type="Integer" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='PointBM'">
        <parameter name="{@name}" type="Point" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='PathBM'">
      <parameter name="{@name}" type="Path" value="{@value}">
        <xsl:apply-templates/>
      </parameter>
      </xsl:when>
      <xsl:when test="@type='AreaBM'">
        <parameter name="{@name}" type="Area" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='AgentBM'">
        <parameter name="{@name}" type="Agent" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='AgentKnowledgeBM'">
        <parameter name="{@name}" type="AgentKnowledge" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:when test="@type='ObjectKnowledgeBM'">
        <parameter name="{@name}" type="AgentKnowledge" value="{@value}">
          <xsl:apply-templates/>
        </parameter>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy>
          <xsl:copy-of select="@*"/>
          <xsl:apply-templates/>
        </xsl:copy>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  </xsl:transform>
