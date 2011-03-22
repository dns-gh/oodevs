<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<missions>
			<xsl:apply-templates/>
		</missions>
	</xsl:template>
	
	<xsl:template match="units">
		<units>
			<xsl:for-each select="mission">
				<mission dia-behavior="{@dia-behavior}" dia-type="{@dia-type}" id="{@id}" name="{@name}">
          <parameter dia-name="dangerDirection_" name="direction dangereuse" optional="false" type="Heading"/>
          <parameter dia-name="phaseLines_" max-occurs="unbounded" min-occurs="1" name="Limas" optional="true" type="PhaseLine"/>
          <parameter dia-name="boundaryLimit1_" name="Limit 1" optional="true" type="Limit"/>
          <parameter dia-name="boundaryLimit2_" name="Limit 2" optional="true" type="Limit"/>
          <parameter dia-name="intelligences_" max-occurs="unbounded" min-occurs="1" name="Renseignements" optional="true" type="Intelligence"/>
          <xsl:apply-templates/>
				</mission>
			</xsl:for-each>
		</units>
	</xsl:template>

  <xsl:template match="automats">
    <automats>
      <xsl:for-each select="mission">
        <mission cdt-dia-behavior="{@cdt-dia-behavior}" dia-type="{@dia-type}" id="{@id}" mrt-dia-behavior="{@mrt-dia-behavior}" name="{@name}">
          <parameter dia-name="dangerDirection_" name="direction dangereuse" optional="false" type="Heading"/>
          <parameter dia-name="phaseLines_" max-occurs="unbounded" min-occurs="1" name="Limas" optional="true" type="PhaseLine"/>
          <parameter dia-name="boundaryLimit1_" name="Limit 1" optional="false" type="Limit"/>
          <parameter dia-name="boundaryLimit2_" name="Limit 2" optional="false" type="Limit"/>
          <parameter dia-name="intelligences_" max-occurs="unbounded" min-occurs="1" name="Renseignements" optional="true" type="Intelligence"/>
          <xsl:apply-templates/>
        </mission>
      </xsl:for-each>
    </automats>
  </xsl:template>

  <xsl:template match="populations">
    <populations>
      <xsl:for-each select="mission">
        <mission dia-behavior="{@dia-behavior}" dia-type="{@dia-type}" id="{@id}" name="{@name}">
          <xsl:apply-templates/>
        </mission>
      </xsl:for-each>
    </populations>
  </xsl:template>

  <xsl:template match="fragorders">
    <fragorders>
      <xsl:for-each select="fragorder">
        <fragorder available-without-mission="{@available-without-mission}" dia-type="{@dia-type}" id="{@id}" name="{@name}">
          <xsl:apply-templates/>
        </fragorder>
      </xsl:for-each>
    </fragorders>
  </xsl:template>

  <xsl:template match="parameter">
    <xsl:copy-of select="."/>
  </xsl:template>
	
</xsl:transform>
