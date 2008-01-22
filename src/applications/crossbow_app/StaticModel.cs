using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

namespace Sword
{
    namespace Crossbow
    {
        [Serializable]
        public sealed class MissionDefinition : Dictionary<string, string>
        {
            public MissionDefinition(XmlNode input)
            {
                XmlNodeList nodes = input.SelectNodes("parameter");
                foreach (XmlNode node in nodes)
                    Add(node.Attributes["name"].Value, node.Attributes["type"].Value);
            }
        }

        public sealed class EntityModel : List<string> { } // $$$$ SBO 2007-09-21: Missions list

        public sealed class StaticModel
        {
            private Dictionary<string, MissionDefinition> m_unitMissions = new Dictionary<string, MissionDefinition>();
            private Dictionary<string, MissionDefinition> m_automatMissions = new Dictionary<string, MissionDefinition>();
            private Dictionary<string, EntityModel> m_unitModels = new Dictionary<string, EntityModel>();
            private Dictionary<string, EntityModel> m_automatModels = new Dictionary<string, EntityModel>();
            private Dictionary<string, string> m_unitTypes = new Dictionary<string, string>();
            private Dictionary<string, string> m_automatTypes = new Dictionary<string, string>();
            private List<string> m_objectTypes = new List<string>();

            #region Initialization
            public StaticModel(WorkspaceConfiguration config)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(config.ExerciseFile);
                string dataset = doc.SelectSingleNode("/exercise/model/@dataset").Value;
                dataset += dataset.Length > 0 ? "/" : "";
                string physical = doc.SelectSingleNode("/exercise/model/@physical").Value;
                physical += physical.Length > 0 ? "/" : "";
                ReadPhysical(config.BuildChildFile("../../data/models/" + dataset + "physical/" + physical + "physical.xml"));
            }

            private void ReadPhysical(string filename)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(filename);
                string rootDir = System.IO.Path.GetDirectoryName(filename) + "/";
                ReadMissions(rootDir + doc.SelectSingleNode("/physical/missions/@file").Value);
                ReadObjects(rootDir + doc.SelectSingleNode("/physical/objects/@file").Value);
                ReadModels(rootDir + doc.SelectSingleNode("/physical/models/@file").Value);
                ReadEntities(rootDir + doc.SelectSingleNode("/physical/units/@file").Value, "unit", ref m_unitTypes);
                ReadEntities(rootDir + doc.SelectSingleNode("/physical/automats/@file").Value, "automat", ref m_automatTypes);
            }

            private void ReadMissions(string filename)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(filename);
                ReadMissions(doc, "units", ref m_unitMissions);
                ReadMissions(doc, "automats", ref m_automatMissions);
            }

            private void ReadMissions(XmlDocument doc, string type, ref Dictionary<string, MissionDefinition> list)
            {
                XmlNodeList nodes = doc.SelectNodes("/missions/" + type + "/mission");
                foreach (XmlNode node in nodes)
                    list.Add(node.Attributes["name"].Value, new MissionDefinition(node));
            }

            private void ReadObjects(string filename)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(filename);
                XmlNodeList list = doc.SelectNodes("/objects/real-objects/object/@type");
                foreach (XmlNode node in list)
                    m_objectTypes.Add(node.Value);
            }

            private void ReadModels(string filename)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(filename);
                ReadModels(doc, "unit", ref m_unitModels);
                ReadModels(doc, "automat", ref m_automatModels);
            }

            private void ReadModels(XmlDocument doc, string type, ref Dictionary<string, EntityModel> list)
            {
                XmlNodeList nodes = doc.SelectNodes("/models/" + type + "s/" + type);
                foreach (XmlNode node in nodes)
                {
                    XmlNodeList missions = node.SelectNodes("missions/mission/@name");
                    EntityModel model = new EntityModel();
                    foreach (XmlNode mission in missions)
                        model.Add(mission.Value);
                    list.Add(node.Attributes["name"].Value, model);
                }
            }

            private void ReadEntities(string filename, string type, ref Dictionary<string, string> list)
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(filename);
                XmlNodeList nodes = doc.SelectNodes("/" + type + "s/" + type);
                foreach (XmlNode node in nodes)
                    list.Add(node.Attributes["name"].Value, node.Attributes["decisional-model"].Value);
            }
            #endregion

            #region Accessors
            public List<string> ObjectTypes
            {
                get
                {
                    return m_objectTypes;
                }
            }

            public Dictionary<string, MissionDefinition> UnitMissions
            {
                get
                {
                    return m_unitMissions;
                }
            }

            public Dictionary<string, MissionDefinition> AutomatMissions
            {
                get
                {
                    return m_automatMissions;
                }
            }

            public Dictionary<string, EntityModel> UnitModels
            {
                get
                {
                    return m_unitModels;
                }
            }

            public Dictionary<string, EntityModel> AutomatModels
            {
                get
                {
                    return m_automatModels;
                }
            }

            public Dictionary<string, string> UnitTypes
            {
                get
                {
                    return m_unitTypes;
                }
            }

            public Dictionary<string, string> AutomatTypes
            {
                get
                {
                    return m_automatTypes;
                }
            }
            #endregion
        }
    }
}