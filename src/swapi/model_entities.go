package swapi

type Formation struct {
	Id         uint32
	PartyId    uint32
	ParentId   uint32
	Name       string
	Formations map[uint32]*Formation
}

func NewFormation(id uint32, name string, parentId uint32,
	partyId uint32) *Formation {
	return &Formation{
		Id:         id,
		PartyId:    partyId,
		ParentId:   parentId,
		Name:       name,
		Formations: map[uint32]*Formation{},
	}
}

func (formation *Formation) Copy() *Formation {
	f := NewFormation(formation.Id, formation.Name, formation.ParentId,
		formation.PartyId)
	for k, v := range formation.Formations {
		f.Formations[k] = v.Copy()
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
