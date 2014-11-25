<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template> 

  <xsl:template match="report">
      <xsl:variable name="report-id" select="@id"/>
      <xsl:copy>
          <xsl:attribute name="key">
            <xsl:choose>
              <xsl:when test="@id=840">eRC_IndirectFireObserved</xsl:when>
              <xsl:when test="@id=841">eRC_BlockedByObject</xsl:when>
              <xsl:when test="@id=842">eRC_UnitNotIdentifying</xsl:when>
              <xsl:when test="@id=843">eRC_ObjectCannotBeBuilt</xsl:when>
              <xsl:when test="@id=844">eRC_CannotWork</xsl:when>
              <xsl:when test="@id=845">eRC_WorkResumptionStart</xsl:when>
              <xsl:when test="@id=857">eRC_NoDeadVictims</xsl:when>
            </xsl:choose>
          </xsl:attribute>
          <xsl:apply-templates select="node()|@*"/>
      </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
