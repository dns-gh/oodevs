<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:template match="text()"/>

	<xsl:template match="/">
		<weapons>
			<xsl:apply-templates />
		</weapons>
	</xsl:template>

	<xsl:template match="Armement">
		<weapon-system launcher="{@lanceur}" munition="{@munition}">
			<burst munition="{NbrMunitionParRafale}" duration="{DureeRafale}"/>
			<reloading munition="{NbrMunitionsParRechargement}" duration="{DureeRechargement}"/>
			<xsl:apply-templates />
		</weapon-system>
	</xsl:template>

	<xsl:template match="Indirect">
		<indirect-fire average-speed="{VitesseMoyenne}" min-range="{PorteeMin}" max-range="{PorteeMax}"/>
	</xsl:template>

	<xsl:template match="Direct/PHs">
		<direct-fire>
			<xsl:apply-templates />
		</direct-fire>
	</xsl:template>

	<xsl:template match="Direct/PHs/VolumeCibleDrone">
		<hit-probabilities target="{substring-after (name(), 'VolumeCible')}">
			<xsl:apply-templates />
		</hit-probabilities>
	</xsl:template>
    <xsl:template match="Direct/PHs/VolumeCibleGros">
		<hit-probabilities target="{substring-after (name(), 'VolumeCible')}">
			<xsl:apply-templates />
		</hit-probabilities>
	</xsl:template>
    <xsl:template match="Direct/PHs/VolumeCibleMoyen">
		<hit-probabilities target="{substring-after (name(), 'VolumeCible')}">
			<xsl:apply-templates />
		</hit-probabilities>
	</xsl:template>
    	<xsl:template match="Direct/PHs/VolumeCiblePetit">
		<hit-probabilities target="{substring-after (name(), 'VolumeCible')}">
			<xsl:apply-templates />
		</hit-probabilities>
	</xsl:template>
    	<xsl:template match="Direct/PHs/VolumeCiblePersonnel">
		<hit-probabilities target="{substring-after (name(), 'VolumeCible')}">
			<xsl:apply-templates />
		</hit-probabilities>
	</xsl:template>
	
	<xsl:template match="PH">
		<hit-probability distance="{@dist}" percentage="{.}"/>
	</xsl:template>
</xsl:transform>
