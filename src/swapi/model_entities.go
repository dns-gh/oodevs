package swapi

type Automat struct {
	Id       uint32
	PartyId  uint32
	Name     string
	Automats []*Automat
}

func NewAutomat(id, partyId uint32, name string) *Automat {
	return &Automat{
		Id:       id,
		PartyId:  partyId,
		Name:     name,
		Automats: []*Automat{},
	}
}

func (a *Automat) Copy() *Automat {
	other := NewAutomat(a.Id, a.PartyId, a.Name)
	for k, v := range a.Automats {
		other.Automats[k] = v.Copy()
	}
	return other
}

type Formation struct {
	Id         uint32
	PartyId    uint32
	ParentId   uint32
	Name       string
	Level      string
	LogLevel   string
	Formations map[uint32]*Formation
	Automats   map[uint32]*Automat
}

func NewFormation(id uint32, name string, parentId uint32,
	partyId uint32, level, logLevel string) *Formation {
	return &Formation{
		Id:         id,
		PartyId:    partyId,
		ParentId:   parentId,
		Name:       name,
		Level:      level,
		LogLevel:   logLevel,
		Formations: map[uint32]*Formation{},
		Automats:   map[uint32]*Automat{},
	}
}

func (formation *Formation) Copy() *Formation {
	f := NewFormation(formation.Id, formation.Name, formation.ParentId,
		formation.PartyId, formation.Level, formation.LogLevel)
	for k, v := range formation.Formations {
		f.Formations[k] = v.Copy()
	}
	for k, v := range formation.Automats {
		f.Automats[k] = v.Copy()
	}
	return f
}

type Party struct {
	Id         uint32
	Name       string
	Formations map[uint32]*Formation
}

func NewParty(id uint32, name string) *Party {
	return &Party{
		Id:         id,
		Name:       name,
		Formations: map[uint32]*Formation{},
	}
}

func (party *Party) Copy() *Party {
	p := NewParty(party.Id, party.Name)
	for k, v := range party.Formations {
		p.Formations[k] = v.Copy()
	}
	return p
}

type ModelData struct {
	Parties map[uint32]*Party
}

func NewModelData() *ModelData {
	return &ModelData{
		Parties: map[uint32]*Party{},
	}
}

func (model *ModelData) Copy() *ModelData {
	m := NewModelData()
	for k, v := range model.Parties {
		m.Parties[k] = v.Copy()
	}
	return m
}

func (model *ModelData) ListFormations() []*Formation {
	formations := []*Formation{}
	pendings := []*Formation{}
	for _, p := range model.Parties {
		for _, f := range p.Formations {
			pendings = append(pendings, f)
		}
	}
	for len(pendings) > 0 {
		f := pendings[len(pendings)-1]
		pendings = pendings[:len(pendings)-1]
		for _, v := range f.Formations {
			pendings = append(pendings, v)
		}
		formations = append(formations, f)
	}
	return formations
}

func (model *ModelData) FindFormation(formationId uint32) *Formation {
	for _, f := range model.ListFormations() {
		if f.Id == formationId {
			return f
		}
	}
	return nil
}

func (model *ModelData) addFormation(f *Formation) bool {
	parent := model.FindFormation(f.ParentId)
	if parent != nil {
		parent.Formations[f.Id] = f
		return true
	}
	party, ok := model.Parties[f.PartyId]
	if ok {
		party.Formations[f.Id] = f
		return true
	}
	return false
}

func (model *ModelData) ListAutomats() []*Automat {
	automats := []*Automat{}
	pendings := []*Automat{}
	for _, f := range model.ListFormations() {
		for _, a := range f.Automats {
			pendings = append(pendings, a)
		}
	}
	for len(pendings) > 0 {
		a := pendings[len(pendings)-1]
		pendings = pendings[:len(pendings)-1]
		for _, child := range a.Automats {
			pendings = append(pendings, child)
		}
		automats = append(automats, a)
	}
	return automats
}

func (model *ModelData) FindAutomat(id uint32) *Automat {
	for _, a := range model.ListAutomats() {
		if a.Id == id {
			return a
		}
	}
	return nil
}

func (model *ModelData) addAutomat(automatId, formationId uint32, a *Automat) bool {
	if parent := model.FindAutomat(automatId); parent != nil {
		parent.Automats[a.Id] = a
		return true
	}
	if parent := model.FindFormation(formationId); parent != nil {
		parent.Automats[a.Id] = a
		return true
	}
	return false
}
