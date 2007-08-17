using System.Collections;
using System.ComponentModel;
using ESRI.ArcGIS.Geodatabase;
using System.Globalization;

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

    public delegate void WorkspaceConfiguration_ConfigurationLoadedEventHandler();

    #region WorkspaceConfiguration
    public sealed class WorkspaceConfiguration
    {
        private LayersConfiguration m_layers = new LayersConfiguration();
        private string m_exerciseFile;
        private string m_workspaceFile;

        public event WorkspaceConfiguration_ConfigurationLoadedEventHandler ConfigurationLoaded;

        public void NotifyLoaded()
        {
            ConfigurationLoaded.Invoke();
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
        private string m_unitForces;
        private string m_tacticalLines;

        [DescriptionAttribute("Layer containing units")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("UnitForces")]
        public string UnitForces
        {
            get
            {
                return m_unitForces;
            }
            set
            {
                m_unitForces = value;
            }
        }

        [DescriptionAttribute("Layer containing tactical lines")]
        [CategoryAttribute("Layer bindings")]
        [TypeConverterAttribute(typeof(FeatureLayerList))]
        [DefaultValue("TacticalLines")]
        public string TacticalLines
        {
            get
            {
                return m_tacticalLines;
            }
            set
            {
                m_tacticalLines = value;
            }
        }
    }
    #endregion
}