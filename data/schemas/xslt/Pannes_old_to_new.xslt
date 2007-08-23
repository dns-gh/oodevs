<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />

	<xsl:template match="Automates">
		<automates>
			<xsl:apply-templates />
		</automates>
	</xsl:template>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<breakdowns>
			<xsl:apply-templates/>
		</breakdowns>
	</xsl:template>
	
	<xsl:template match="TempsDiagnostique">
		<diagnosis time="{.}"/>
	</xsl:template>
	
	<xsl:template match="Types">
		<xsl:for-each select="./*">
			<category name="{name()}">
				<xsl:apply-templates/>
			</category>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template match="Panne">
		<breakdown name="{@nom}" type="{@type}" id="{MosID}" average-repairing-time="{Reparation/@tempsMoyen}" variance="{Reparation/@variance}">
			<xsl:apply-templates/>
		</breakdown>
	</xsl:template>
	
	<xsl:template match="Pieces/Piece">
		<part dotation="{@categorie}" quantity="{.}"/>
	</xsl:template>
	
	</xsl:transform>