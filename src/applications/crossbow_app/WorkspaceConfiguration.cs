using System;
using System.Collections;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using ESRI.ArcGIS.Geodatabase;

namespace Crossbow
{
    #region FeatureLayerList
    public sealed class FeatureLayerList : StringConverter
    {
        private static ArrayList m_values = new ArrayList();

        public static void SetValues(IEnumDatasetName names)
        {
            m_values.Clear();
            IDatasetName name;
            while ((name = names.Next()) != null)
                m_values.Add(name.Name);
        }

        #region StringConverter override
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(m_values);
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return true;
        }
        #endregion
    }
    #endregion

    #region WorkspaceConfiguration
    public sealed class WorkspaceConfiguration
    {
        private LayersConfiguration m_layers = new LayersConfiguration();
        private string m_exerciseFile;
        private string m_workspaceFile;
        private string m_sharedFile;

        public event EventHandler ConfigurationLoaded;

        public void NotifyLoaded()
        {
            ConfigurationLoaded(this, EventArgs.Empty);
        }

        public string ExerciseFile
        {
            get
            {
                return m_exerciseFile;
            }
            set
            {
                m_exerciseFile = value;
            }
        }

        private static string ExtractDirectory(string filename)
        {
            return Path.GetDirectoryName(filename);
        }

        public string BuildChildFile(string filename)
        {
            return Path.GetFullPath(ExtractDirectory(m_exerciseFile) + "/" + filename);
        }

        public string WorkspaceFile
        {
            get
            {
                return m_workspaceFile;
            }
            set
            {
                m_workspaceFile = value;
            }
        }

        public string SharedFile
        {
            get
            {
                return m_sharedFile;
            }
            set
            {
                m_sharedFile = value;
            }
        }

        public LayersConfiguration LayersConfiguration
        {
            get
            {
                return m_layers;
            }
        }
    }
    #endregion

    #region LayersConfiguration
    public sealed class LayersConfiguration
    {
        private string m_units  = "UnitForces";
        private string m_limits = "BoundaryLimits";
        private string m_limas = "TacticalLines";
        private string m_reports = "Reports";
        private string m_objectsPoint = "TacticalObjectPoint";

        [DescriptionAttribute("Layer containing units")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("UnitForces")]
        public string Units
        {
            get
            {
                return m_units;
            }
            set
            {
                m_units = value;
            }
        }

        [DescriptionAttribute("Layer containing tactical lines")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("TacticalLines")]
        public string Limas
        {
            get
            {
                return m_limas;
            }
            set
            {
                m_limas = value;
            }
        }

        [DescriptionAttribute("Layer containing boundary limits")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("BoundaryLimits")]
        public string Limits
        {
            get
            {
                return m_limits;
            }
            set
            {
                m_limits = value;
            }
        }

        [DescriptionAttribute("Layer containing tactical objects")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("TacticalObjectPoint")]
        public string TacticalObjectPoint
        {
            get
            {
                return m_objectsPoint;
            }
            set
            {
                m_objectsPoint = value;
            }
        }

        [DescriptionAttribute("Table containing reports")]
        [CategoryAttribute("Table bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("Reports")]
        public string Reports
        {
            get
            {
                return m_reports;
            }
            set
            {
                m_reports = value;
            }
        }
       
    }
    #endregion
}